some improvements to the uno code...

I think I have found some improvements that enable more samples without unwanted odds. So I changed the array size that stores the total time for the events from uint32_t to "uint24_t" (;-)

Because there ist no uint24_t I have splitted the total time into two arrays. One uint16_t holds the TCNT1 and a uint8_t holds the overflow counter. So the maximum time of total_time is 8 seconds.

With this changes there are 400 samples possible and the output of the UNO is not changed to the original code.

Further more the myMicros () is smaller and less time consumpting because the calculations for the total time are made while outputting to the pc.