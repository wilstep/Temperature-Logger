#include "tlog.h"

// Data to calibrate device using a linear least squares fit.
// Ncal is number of calibration points, must be 2 or more.
// Arrays T_cal and cnt_cal must form an ordered pair each 
// with Ncal elements
#define Ncal 3
static double T_cal[] = {26.7, 11.3, 15.4};
static double cnt_cal[] = {2111., 1596., 1733.45};
static double a, b; // parameters for straight line, T = a * nav + b
// where nav is the number off the BBB's ADC as read in GetTemp

void LeastSq(void) // Find the straight line least square fit for T_cal vs cnt_cal
{
    int i;
    double Tbar, cbar, x;
    
    Tbar = cbar = 0.0;
    for(i=0; i<Ncal; ++i){
        Tbar += T_cal[i];
        cbar += cnt_cal[i];
    }
    Tbar /= (double) Ncal;
    cbar /= (double) Ncal;
    a = b = 0.0;
    for(i=0; i<Ncal; ++i){
        x = (cnt_cal[i] - cbar);
        a += (T_cal[i] - Tbar) * x;
        b += x * x;    
    }
    a = a / b;
    b = Tbar - a * cbar;  
}

FILE *gfopen(char *fn, char *mode) // graceful file open
{
    FILE *fp;
    
    if((fp = fopen(fn, mode)) == NULL){
        fprintf(stderr, "Cannot open %s - bye!\n", fn);
        exit(1);
    }
    return fp;
}

double getTemp(int cal_b)
{
    FILE *ifp;
    int i, n;
    double nav;
    double T, V;
    
    if(access(tfile, F_OK) == -1){
        fprintf(stderr, "FILE: \"%s\" does not exist- bye!\n", tfile);
        exit(1);
    }
    nav = 0.0;
    for(i=0; i<av_N; ++i){   
        ifp = gfopen(tfile, "r");
        fscanf(ifp, "%d", &n);
        fclose(ifp); 
        nav += (double) n;
        if(i != av_N-1) usleep(500000);
    }
    nav /= (double) av_N;
    T = a * nav + b;
    V = nav / 4095.0 * 1.8;
    if(cal_b) printf("n = %f, V = %f, T = %f\n", nav, V, T);
    return T; 
}


static int Nsamps = 1000;
static int period = 15; // sample every 'period' minutes 


int main()
{
    int i;
    int secs = period * 60;
    double T=0;
    double rem;
    time_t t;
    struct tm tm; 
    FILE *ofp;
    
    LeastSq();
    if(access("tlog.csv", F_OK) == -1){
        ofp = gfopen("tlog.csv", "w");
        fprintf(ofp, "temperature,date,24hr time\n");
    }
    else ofp = gfopen("tlog.csv", "a");
    for(i=0; i<Nsamps; ++i){
        T = getTemp(1);
        t =  time(NULL);
        tm = *localtime(&t);
        fprintf(ofp, "%.1f,%.2d-%.2d-%d,%.2d:%.2d\n", 
            T, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
        fflush(ofp);
        sleep(secs);
    }
    fclose(ofp);
    return 0;
}
