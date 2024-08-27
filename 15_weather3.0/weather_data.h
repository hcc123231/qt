#ifndef WEATHER_DATE_H
#define WEATHER_DATE_H
#include<QString>
class Dataset{
public:
    Dataset(){

    };
    ~Dataset(){

    };
    Dataset(int high,int low,QString ymd,QString week,int aqi,QString fx,
            QString fl,QString type){
        m_high=high;
        m_low=low;
        m_ymd=ymd;
        m_week=week;
        m_aqi=aqi;
        m_fx=fx;
        m_fl=fl;
        m_type=type;
    }
    int m_high;
    int m_low;
    QString m_ymd;
    QString m_week;
    int m_aqi;
    QString m_fx;
    QString m_fl;
    QString m_type;
};


#endif // WEATHER_DATE_H



