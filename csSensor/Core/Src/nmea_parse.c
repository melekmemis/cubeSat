//
// Created by sztuka on 22.01.2023.
//
#include "nmea_parse.h"



char *data[15];

int gps_checksum(char *nmea_data)
{
    //if you point a string with less than 5 characters the function will read outside of scope and crash the mcu.
    if(strlen(nmea_data) < 5) return 0;
    char recv_crc[2];
    recv_crc[0] = nmea_data[strlen(nmea_data) - 4];
    recv_crc[1] = nmea_data[strlen(nmea_data) - 3];
    int crc = 0;
    int i;

    //exclude the CRLF plus CRC with an * from the end
    for (i = 0; i < strlen(nmea_data) - 5; i ++) {
        crc ^= nmea_data[i];
    }
    int receivedHash = strtol(recv_crc, NULL, 16);
    if (crc == receivedHash) {
        return 1;
    }
    else{
        return 0;
    }
}

int nmea_GPGGA(GPS *gps_data, char*inputString){
//    char *values[25];
//    int counter = 0;
//    memset(values, 0, sizeof(values));
//    char *marker = strtok(inputString, ",");
//    while (marker != NULL) {
//        values[counter++] = malloc(strlen(marker) + 1); //free later!!!!!!
//        strcpy(values[counter - 1], marker);
//        marker = strtok(NULL, ",");
//    }
//    char lonSide = values[5][0];
//    char latSide = values[3][0];
//    strcpy(gps_data->lastMeasure, values[1]);
//    if(latSide == 'S' || latSide == 'N'){
//        char lat_d[2];
//        char lat_m[7];
//        for (int z = 0; z < 2; z++) lat_d[z] = values[2][z];
//        for (int z = 0; z < 6; z++) lat_m[z] = values[2][z + 2];
//
//        int lat_deg_strtol = strtol(lat_d, NULL, 10);
//        float lat_min_strtof = strtof(lat_m, NULL);
//        double lat_deg = lat_deg_strtol + lat_min_strtof / 60;
//
//        char lon_d[3];
//        char lon_m[7];
//
//        for (int z = 0; z < 3; z++) lon_d[z] = values[4][z];
//        for (int z = 0; z < 6; z++) lon_m[z] = values[4][z + 3];
//
//        int lon_deg_strtol = strtol(lon_d, NULL, 10);
//        float lon_min_strtof = strtof(lon_m, NULL);
//        double lon_deg = lon_deg_strtol + lon_min_strtof / 60;
//
//        if(lat_deg!=0 && lon_deg!=0 && lat_deg<90 && lon_deg<180){
//            gps_data->latitude = lat_deg;
//            gps_data->latSide = latSide;
//            gps_data->longitude = lon_deg;
//            gps_data->lonSide = lonSide;
//            float altitude = strtof(values[9], NULL);
//            gps_data->altitude = altitude!=0 ? altitude : gps_data->altitude;
//            gps_data->satelliteCount = strtol(values[7], NULL, 10);
//
//            int fixQuality = strtol(values[6], NULL, 10);
//            gps_data->fix = fixQuality > 0 ? 1 : 0;
//
//            float hdop = strtof(values[8], NULL);
//            gps_data->hdop = hdop!=0 ? hdop : gps_data->hdop;
//        }
//        else {
//            for(int i=0; i<counter; i++) free(values[i]);
//            return 0;
//        }
//
//    }
//
//    for(int i=0; i<counter; i++) free(values[i]);
//    return 1;

	char values[25][20]; // Her bir parçayı 20 karaktere kadar saklayabilecek sabit alan
	    int counter = 0;

	    char *marker = strtok(inputString, ",");
	    while (marker != NULL && counter < 25) {
	        strncpy(values[counter], marker, sizeof(values[counter]) - 1);
	        values[counter][sizeof(values[counter]) - 1] = '\0'; // Güvenlik için null-terminator
	        marker = strtok(NULL, ",");
	        counter++;
	    }

	    char lonSide = values[5][0];
	    char latSide = values[3][0];
	    strcpy(gps_data->lastMeasure, values[1]);

	    if(latSide == 'S' || latSide == 'N'){
	        char lat_d[3] = {0};
	        char lat_m[8] = {0};
	        strncpy(lat_d, values[2], 2);
	        strncpy(lat_m, values[2] + 2, 6);

	        int lat_deg_strtol = strtol(lat_d, NULL, 10);
	        float lat_min_strtof = strtof(lat_m, NULL);
	        double lat_deg = lat_deg_strtol + lat_min_strtof / 60.0;

	        char lon_d[4] = {0};
	        char lon_m[8] = {0};
	        strncpy(lon_d, values[4], 3);
	        strncpy(lon_m, values[4] + 3, 6);

	        int lon_deg_strtol = strtol(lon_d, NULL, 10);
	        float lon_min_strtof = strtof(lon_m, NULL);
	        double lon_deg = lon_deg_strtol + lon_min_strtof / 60.0;

	        if(lat_deg != 0 && lon_deg != 0 && lat_deg < 90 && lon_deg < 180){
	            gps_data->latitude = lat_deg;
	            gps_data->latSide = latSide;
	            gps_data->longitude = lon_deg;
	            gps_data->lonSide = lonSide;

	            float altitude = strtof(values[9], NULL);
	            gps_data->altitude = altitude != 0 ? altitude : gps_data->altitude;
	            gps_data->satelliteCount = strtol(values[7], NULL, 10);

	            int fixQuality = strtol(values[6], NULL, 10);
	            gps_data->fix = fixQuality > 0 ? 1 : 0;

	            float hdop = strtof(values[8], NULL);
	            gps_data->hdop = hdop != 0 ? hdop : gps_data->hdop;

	            return 1;
	        } else {
	            return 0;
	        }
	    }

	    return 0;
}


int nmea_GPGSA(GPS *gps_data, char*inputString){
//    char *values[25];
//    int counter = 0;
//    memset(values, 0, sizeof(values));
//    char *marker = strtok(inputString, ",");
//    while (marker != NULL) {
//        values[counter++] = malloc(strlen(marker) + 1); //free later!!!!!!
//        strcpy(values[counter - 1], marker);
//        marker = strtok(NULL, ",");
//    }
//    int fix = strtol(values[2], NULL, 10);
//    gps_data->fix = fix > 1 ? 1 : 0;
//    int satelliteCount = 0;
//    for(int i=3; i<15; i++){
//        if(values[i][0] != '\0'){
//            satelliteCount++;
//        }
//    }
//    gps_data->satelliteCount = satelliteCount;
//    for(int i=0; i<counter; i++) free(values[i]);
//    return 1;

#define MAX_VALUES 25
#define MAX_TOKEN_LENGTH 20

char values[MAX_VALUES][MAX_TOKEN_LENGTH];
int counter = 0;

char *marker = strtok(inputString, ",");
while (marker != NULL && counter < MAX_VALUES) {
    strncpy(values[counter], marker, MAX_TOKEN_LENGTH - 1);
    values[counter][MAX_TOKEN_LENGTH - 1] = '\0';
    counter++;
    marker = strtok(NULL, ",");
}

// Fix type (0 = none, 1 = 2D, 2 = 3D)
int fix = strtol(values[2], NULL, 10);
gps_data->fix = fix > 1 ? 1 : 0;

// Uydu sayısı (index 3–14 arası dolu olanlar)
int satelliteCount = 0;
for (int i = 3; i <= 14 && i < counter; i++) {
    if (values[i][0] != '\0') {
        satelliteCount++;
    }
}
gps_data->satelliteCount = satelliteCount;

return 1;

}



int nmea_GPGLL(GPS *gps_data, char*inputString) {

//    char *values[25];
//    int counter = 0;
//    memset(values, 0, sizeof(values));
//    char *marker = strtok(inputString, ",");
//    while (marker != NULL) {
//        values[counter++] = malloc(strlen(marker) + 1); //free later!!!!!!
//        strcpy(values[counter - 1], marker);
//        marker = strtok(NULL, ",");
//    }
//    char latSide = values[2][0];
//    if (latSide == 'S' || latSide == 'N') { //check if data is sorta intact
//        char lat_d[2];
//        char lat_m[7];
//        for (int z = 0; z < 2; z++) lat_d[z] = values[1][z];
//        for (int z = 0; z < 6; z++) lat_m[z] = values[1][z + 2];
//
//        int lat_deg_strtol = strtol(lat_d, NULL, 10);
//        float lat_min_strtof = strtof(lat_m, NULL);
//        double lat_deg = lat_deg_strtol + lat_min_strtof / 60;
//
//        char lon_d[3];
//        char lon_m[7];
//        char lonSide = values[4][0];
//        for (int z = 0; z < 3; z++) lon_d[z] = values[3][z];
//        for (int z = 0; z < 6; z++) lon_m[z] = values[3][z + 3];
//
//        int lon_deg_strtol = strtol(lon_d, NULL, 10);
//        float lon_min_strtof = strtof(lon_m, NULL);
//        double lon_deg = lon_deg_strtol + lon_min_strtof / 60;
//        //confirm that we aren't on null island
//        if(lon_deg_strtol == 0 || lon_min_strtof == 0 || lat_deg_strtol == 0 || lat_min_strtof == 0) {
//            for(int i = 0; i<counter; i++) free(values[i]);
//            return 0;
//        }
//        else{
//            gps_data->latitude = lat_deg;
//            gps_data->longitude = lon_deg;
//            gps_data->latSide = latSide;
//            gps_data->lonSide = lonSide;
//            for(int i = 0; i<counter; i++) free(values[i]);
//            return 1;
//        }
//    }
//    else return 0;

	char values[25][20]; // Her alan için sabit buffer
	    int counter = 0;

	    char *marker = strtok(inputString, ",");
	    while (marker != NULL && counter < 25) {
	        strncpy(values[counter], marker, sizeof(values[counter]) - 1);
	        values[counter][sizeof(values[counter]) - 1] = '\0'; // null-terminator güvenliği
	        marker = strtok(NULL, ",");
	        counter++;
	    }

	    char latSide = values[2][0];
	    if (latSide == 'S' || latSide == 'N') { // veri geçerlilik kontrolü
	        char lat_d[3] = {0};
	        char lat_m[8] = {0};
	        strncpy(lat_d, values[1], 2);
	        strncpy(lat_m, values[1] + 2, 6);

	        int lat_deg_strtol = strtol(lat_d, NULL, 10);
	        float lat_min_strtof = strtof(lat_m, NULL);
	        double lat_deg = lat_deg_strtol + lat_min_strtof / 60.0;

	        char lon_d[4] = {0};
	        char lon_m[8] = {0};
	        strncpy(lon_d, values[3], 3);
	        strncpy(lon_m, values[3] + 3, 6);
	        char lonSide = values[4][0];

	        int lon_deg_strtol = strtol(lon_d, NULL, 10);
	        float lon_min_strtof = strtof(lon_m, NULL);
	        double lon_deg = lon_deg_strtol + lon_min_strtof / 60.0;

	        // Null island kontrolü
	        if (lon_deg_strtol == 0 || lon_min_strtof == 0 || lat_deg_strtol == 0 || lat_min_strtof == 0) {
	            return 0;
	        } else {
	            gps_data->latitude = lat_deg;
	            gps_data->longitude = lon_deg;
	            gps_data->latSide = latSide;
	            gps_data->lonSide = lonSide;
	            return 1;
	        }
	    }

	    return 0;
}

void nmea_parse(GPS *gps_data, uint8_t *buffer){
    memset(data, 0, sizeof(data));
    char * token = strtok(buffer, "$");
    int cnt = 0;
//    while(token !=NULL){
//        data[cnt++] = malloc(strlen(token)+1); //free later!!!!!
//        strcpy(data[cnt-1], token);
//        token = strtok(NULL, "$");
//    }

    while (token != NULL) {
            if (cnt < 15) {  // data array'inin boyutunu aşmamak için kontrol
                data[cnt++] = token;  // Bu durumda malloc kullanmanıza gerek yok
            }
            token = strtok(NULL, "$");
        }

    for(int i = 0; i<cnt; i++){
       if(strstr(data[i], "\r\n")!=NULL && gps_checksum(data[i])){
           if(strstr(data[i], "GPGLL")!=NULL){
               nmea_GPGLL(gps_data, data[i]);
           }
           else if(strstr(data[i], "GPGSA")!=NULL){
               nmea_GPGSA(gps_data, data[i]);
           }
//           if(strstr(data[i], "GPGSA")!=NULL){
//                 nmea_GPGSA(gps_data, data[i]);
//           }
           else if(strstr(data[i], "GPGGA")!=NULL){
               nmea_GPGGA(gps_data, data[i]);
           }
       }

    }

//    for(int i = 0; i<cnt; i++) free(data[i]);


}
