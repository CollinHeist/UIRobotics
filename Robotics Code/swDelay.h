/* ---------------------- Guard against multiple inclusion ------------------- */
#ifndef _SW_DELAY_H
    #define _SW_DELAY_H

    /* ---------------------- Public Function declarations ------------------- */
    void DelayMs(unsigned int mS);
    void DelayUs(unsigned int us);
    void DelayNs(unsigned int ns);
    int PeriodMs(unsigned int msec);
#endif