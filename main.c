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

void assign_value_from_index(struct Weather * data , int index , const char * token) { // used in read_csv

    const int size_dt_iso = 30, size_city_name = 50,size_weather_main=20,size_weather_description=50,size_weather_icon=5;
    int /*i_dt=0,*/i_dt_iso=1,i_timezone=2,i_city_name=3,i_lat=4,i_lon=5,i_temp=6,i_visibility=7,i_dew_point=8,i_feels_like=9;
    int i_temp_min=10,i_temp_max=11,i_pressure=12,i_sea_level=13,i_grnd_level=14,i_humidity=15,i_wind_speed=16,i_wind_deg=17;
    int i_wind_gust=18,i_rain_1h=19,i_rain_3h=20,i_snow_1h=21,i_snow_3h=22,i_clouds_all=23,i_weather_id=24,i_weather_main=25;
    int i_weather_description=26, i_weather_icon=27;

    if (index==i_dt_iso) {
        if (strlen(token)<size_dt_iso)
            strcpy(data->dt_iso,token);
        else
            strcpy(data->dt_iso,"[OUT]");
    }
    if (index==i_timezone) {
        data->timezone=atoi(token);
    }
    if (index==i_city_name) {
        if (strlen(token)<size_city_name)
            strcpy(data->city_name,token);
        else
            strcpy(data->city_name,"[OUT]");
    }
    if (index==i_lat) {
        data->lat=atof(token);
    }
    if (index==i_lon) {
        data->lon=atof(token);
    }
    if (index==i_temp) {
        data->temp=atof(token);
    }
    if (index==i_temp_max) {
        data->temp_max=atof(token);
    }
    if (index==i_temp_min) {
        data->temp_min=atof(token);
    }
    if (index==i_pressure) {
        data->pressure=atoi(token);
    }
    if (index==i_sea_level) {
        data->sea_level=atoi(token);
    }
    if (index==i_grnd_level) {
        data->grnd_level=atoi(token);
    }
    if (index==i_humidity) {
        data->humidity=atoi(token);
    }
    if (index==i_wind_speed) {
        data->wind_speed=atof(token);
    }
    if (index==i_wind_deg) {
        data->wind_deg=atoi(token);
    }
    if (index==i_wind_gust) {
        data->wind_gust=atof(token);
    }
    if (index==i_rain_1h) {
        data->rain_1h=atof(token);
    }
    if (index==i_rain_3h) {
        data->rain_3h=atof(token);
    }
    if (index==i_snow_1h) {
        data->snow_1h=atof(token);
    }
    if (index==i_snow_3h) {
        data->snow_3h=atof(token);
    }
    if (index==i_clouds_all) {
        data->clouds_all=atoi(token);
    }
    if (index==i_weather_id) {
        data->weather_id=atoi(token);
    }
    if (index==i_weather_main) {
        if (strlen(token)<size_weather_main)
            strcpy(data->weather_main,token);
        else
            strcpy(data->weather_main,"[OUT]");
    }
    if (index==i_weather_description) {
        if (strlen(token)<size_weather_description)
            strcpy(data->weather_description,token);
        else
            strcpy(data->weather_description,"[OUT]");
    }
    if (index==i_weather_icon) {
        if (strlen(token)<size_weather_icon)
            strcpy(data->weather_icon,token);
        else
            strcpy(data->weather_icon,"[OUT]");
    }
    if (index==i_visibility) {
        data->visibility=atof(token);
    }
    if (index==i_dew_point) {
        data->dew_point=atof(token);
    }
    if (index==i_feels_like) {
        data->feels_like=atof(token);
    }

}

int read_csv(const char *filename, struct Weather **data, int *size) {
    char line[CHUNK];
    *size = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    *data = (struct Weather *)malloc(sizeof(struct Weather));
    if (*data == NULL) {
        printf("Memory allocation error\n");
        return -1;
    }

    fgets(line, CHUNK, file); // don't care about first line
    while (fgets(line, CHUNK, file)!=NULL) {
        *data = (struct Weather *)realloc(*data, ((*size)+1) * sizeof(struct Weather));
        if ((*data)==NULL) {
            printf("Memory allocation error\n");
            return -1;
        }
        char *tok = strtok(line, ",");
        int index=0;
        while (tok != NULL) {
           assign_value_from_index((*data)+*size, index, tok);  // to do
            ++index;
            tok = strtok(NULL, ",");
        }
        ++*size;
        printf("%d lines processed\n",*size);

    }
    fclose(file);
    return 1;
}

void display_statistics(struct Weather *data, int size);
struct Weather *filter_by_date(struct Weather *data, int size, time_t start, time_t end, int *result_size);
void draw_temperature_chart(struct Weather *data, int size);
void free_weather_data(struct Weather *data);

void print_all_test(struct Weather *data,int size);

struct Weather *data;

int main() {
    int size = 0;
    // const char* path = "Timisoara.csv";
    const char* path = "plm.txt";
    if (read_csv(path, &data, &size)==(-1)) {
        exit(-1);
    }

    print_all_test(data,size);

    free(data);
    return 0;
}

void print_all_test(struct Weather *data,int size) {
    for (int i = 0; i < size; i++) {
        printf("Dt_iso: %s\n", data[i].dt_iso);
        printf("Time: %d\n", data[i].timezone);
        printf("Name: %s\n", data[i].city_name);
        printf("Lon: %lf\n", data[i].lon);
        printf("Lat: %lf\n", data[i].lat);
        printf("Temp: %lf\n", data[i].temp);
        printf("Visibility: %lf\n", data[i].visibility);
        printf("dew: %lf\n", data[i].dew_point);
        printf("feel_like: %lf\n", data[i].feels_like);
        printf("Temp_min:%lf\n", data[i].temp_min);
        printf("Temp_max:%lf\n", data[i].temp_max);
        printf("Pressure: %d\n", data[i].pressure);
        printf("Sea: %d\n", data[i].sea_level);
        printf("grnd: %d\n", data[i].grnd_level);
        printf("humid: %d\n", data[i].humidity);
        printf("wind_speed: %lf\n", data[i].wind_speed);
        printf("wind_deg: %d\n", data[i].wind_deg);
        printf("wind_gust: %lf\n", data[i].wind_gust);
        printf("rain_1h: %lf\n", data[i].rain_1h);
        printf("rain_3h: %lf\n", data[i].rain_3h);
        printf("snow_1h: %lf\n", data[i].snow_1h);
        printf("snow_3h: %lf\n", data[i].snow_3h);
        printf("clouds_all: %d\n", data[i].clouds_all);
        printf("weather_id: %d\n", data[i].weather_id);
        printf("weather_main: %s\n", data[i].weather_main);
        printf("weather_description: %s\n", data[i].weather_description);
        printf("weather_icon: %s\n", data[i].weather_icon);
        printf("\n");
    }
}
