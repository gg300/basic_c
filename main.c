#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 1024

struct Weather {
    time_t timestamp;
    char dt_iso[30];
    int timezone;
    char city_name[50];
    double lat, lon;
    double temp, visibility, dew_point, feels_like, temp_min, temp_max;
    int pressure, sea_level, grnd_level, humidity;
    double wind_speed;
    int wind_deg;
    double wind_gust, rain_1h, rain_3h, snow_1h, snow_3h;
    int clouds_all, weather_id;
    char weather_main[20];
    char weather_description[50];
    char weather_icon[5];
};

void assign_value_from_index(struct Weather * data , int index) { //to do

}

int read_csv(const char *filename, struct Weather **data, int *size) {
    int size_dt_iso = 30, size_city_name = 50,size_weather_main=20,size_weather_description=50,size_weather_icon=5;
    int i_dt=0,i_dt_iso=1,i_timezone=2,i_city_name=3,i_lat=4,i_lon=5,i_temp=6,i_visibility=7,i_dew_point=8,i_feels_like=9;
    // int i_temp_min=10,i_temp_max=11,i_pressure=12,i_sea_level=13,i_grnd_level=14,i_humidity=15,i_wind_speed=16,i_wind_deg=17;
    // int i_wind_gust=18,i_rain_1h=19,i_rain_3h=20,i_snow_1h=21,i_snow_3h=22,i_clouds_all=23,i_weather_id=24,i_weather_main=25;
    // int i_weather_description=26, i_weather_icon=27;
    *size = 0;
    *data = (struct Weather *)malloc(sizeof(struct Weather));
    char line[CHUNK];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    fgets(line, CHUNK, file); // don't care about first line

    // while (tok != NULL) {
    //     printf("%s\n", tok);
    //     tok = strtok(NULL, ",");
    // }

    while (fgets(line, CHUNK, file)!=NULL) {
        *data = (struct Weather *)realloc(*data, ((*size)+1) * sizeof(struct Weather));
        if ((*data)==NULL) {
            printf("Memory allocation error\n");
            return -1;
        }
        char *tok = strtok(line, ",");
        int index=0;
        while (tok != NULL) {
            if (index==i_dt_iso) {
                if (strlen(tok)<size_dt_iso)
                    strcpy((*data)[*size].dt_iso,tok);
            }
           // assign_value_from_index((*data)+*size, index);  // to do

            ++index;
            tok = strtok(NULL, ",");
        }
        ++*size;
    }


    fclose(file);
    return 1;
}

void display_statistics(struct Weather *data, int size);
struct Weather *filter_by_date(struct Weather *data, int size, time_t start, time_t end, int *result_size);
void draw_temperature_chart(struct Weather *data, int size);
void free_weather_data(struct Weather *data);


struct Weather *data;

int main() {
    int size = 0;
    const char* path = "plm.txt";
    if (read_csv(path, &data, &size)==(-1)) {
        exit(-1);
    }

    for (int i = 0; i < size; i++) {
        printf("%s\n", data[i].dt_iso);
    }

    free(data);
    return 0;
}
