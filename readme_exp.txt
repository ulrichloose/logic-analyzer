some experimental changes...

I think the initial idea to store the times of an event instead of storing "millions" of samples what the "traditional" analizers do, is the best and only way to do it with an Arduino as the sampling device.

The cons of the initial idea is the use of uint32_t for the timer array that only can store 250 events on an uno aka 328p.

So my idea is, not to store the total time that counts from the fist event up to the last one, instead I am storing the time-difference between two events. With an uint16_t for the timer array the time-difference between two events can be a max of 65336 uS before an overflow occurs.

The pros - with unint16_t you can store 500 events. The cons - if two events have a time-difference greater than 65 mS an overflow / error occurs.

