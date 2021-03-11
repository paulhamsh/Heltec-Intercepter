# Heltec-Intercepter

See Stick Intercepter for details     

```
Connect pins  

GND - GND   
3.3v - 3.3v   
14 - 14   
12 - 12   

27 - 27
```
14 and 12 are remapped for serial traffic.    
Yes - connect 14 to 14 - the two programs use different pins for send and receive, so no need to cross the wires.   

As with all Arduino GUI code, each file needs to be in a directory of the same name - so you need to do that manually.   

Also, to use:

1. Don't connect the Heltecs YET!  
2. Power up the 'App - Heltec' device, ensure the amp is off, on your Android device pair bluetooth to 'Spark 40 Audio'   
3. Power off    
4. Connect the two Heltecs    
5. Power up one of the Heltecs - the other will also start      
6. Hold the reboot button on the 'Heltec - Spark' device so it doesn't boot     
7. Wait for 'Heltec - Spark' to say connected   
8. Release the boot button   
9. Fire up the Spark app and 'Connect'   
