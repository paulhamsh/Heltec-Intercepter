# Heltec-Intercepter

See Stick Intercepter for details    

![Spark Setups](https://github.com/paulhamsh/Stick-Intercepter/blob/main/M5Sticks.jpg)


```
Connect pins  

GND - GND   
3.3v - 3.3v   
14 - 14   
12 - 12   

27 - 27
```
14 and 12 are remapped for serial traffic   
27 to 27 is to sync between the devices

Yes - connect 14 to 14 - the two programs use different pins for send and receive, so no need to cross the wires.   

As with all Arduino GUI code, each file needs to be in a directory of the same name - so you need to do that manually.   

Also, to use:

1. Connect the Heltecs - you need 5 cables   
2. Connect power to one Heltec   
3. Turn on the amp
4. Wait for Heltec-Spark to pair to the amp   
5. App-Heltec will say it has stopped waiting    
6. Turn on your Android device   
7. Unpair any Spark bluetooth   
8. Scan   
9. Pair Spark 40 Audio   
10. Start the Spark app   
11. Connect   

Try until it works - you should see 3 byte messages scrolling over both screens
