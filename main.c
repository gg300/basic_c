#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int read_csv(const char *filename, struct Weather **data, int *size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    *size = 15;
    fclose(file);
    return 1;
}

void display_statistics(struct Weather *data, int size);
struct Weather *filter_by_date(struct Weather *data, int size, time_t start, time_t end, int *result_size);
void draw_temperature_chart(struct Weather *data, int size);
void free_weather_data(struct Weather *data);

int main() {
    int size = 0;
    struct Weather *data;
    const char* path = "Timisoara.csv";
    if (read_csv(path, &data, &size)==(-1)) {
        exit(-1);
    }
    printf("%d",size);
    return 0;
}
