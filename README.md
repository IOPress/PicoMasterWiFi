# PicoMasterWiFi
Note: These programs have been updated to sdk 2.3 and hence mbedtls 3.2
The use of  MBEDTLS_ALLOW_PRIVATE_ACCESS is a short term fix. 
Mbedtls 3.x has converted many struct fields into private fields that can only be accessed by get/set functions. 
If you want to modernise the code you need to get rid of the define and change direct access into set/get functions.
Which is what will happen in the next edition.

You can browse the repo if you simply want to read the code.
If you want to use the repo to run the code then you can either
download it as a zip file and unzip it to a suitable directory or
you can use Source Control and Clone repo using a URL.

Once you have the files installed you can work with any of the examples
by opening the folder with the correct chapter number and then the folder of the
example you want to work with. Notice that examples in different chapters can
have the same name.

Once you have the folder open you may have to import the project if
it isn't recognized as a Pico project.

You might also have to delete the build directory and use the Clean Cmake command.
You almost certainly will need to select the latest sdk to compile them against.

There are also various configuration details that have to be entered for the programs to
work. In particular you need to supply and ssid and password for the WiFi AP you 
want to connect to. 

In general if you see any errors about not being able to find a file check the
path is correct on your machine. 

Also check the IP of example.com if it is used in the example as it changes frequently.
