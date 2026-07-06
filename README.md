# PicoMasterWiFi
Note: These programs are currently being updated to sdk 2.3 and hence mbedtls 3.2
the update has reached C5 which is a work in progress. Beyond C4 all programs work on sdk 2.1.
You need to make sure that the correct sdk is selected before you build a program.

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

There are also various configuration details that have to be entered for the programs to
work. In particular you need to supply and ssid and password for the WiFi AP you 
want to connect to. 
In general if you see any errors about not being able to find a file check the
path is correct on your machine. 
