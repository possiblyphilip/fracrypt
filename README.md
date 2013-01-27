fracrypt
========

If you encrypt data and lose the key you used, IT IS GONE.

HOW TO USE THE PROGRAM:

At start up fracrypt will show a text image of the Mandelbrot set.

you log in by picking a sub window of the Mandelbrot set shown on screen. This will be your master key which encrypts
or decrypts data with other keys you have created while logged in with this master key. if you use the default master 
key it will not be secure. if you log in with a different master key only the keys you had previously created with that
master key will decrypt the messages or data they have already encrypted. if you encrypt data without first entering or
creating a key, that data will be lost forever because it will be encrypted with an uninitilized key.

TO ENTER A SUB WINDOW:

the sub window is imput in this order

  top left
  top right
  bottom left
  bottom right

each point takes a "char int" pair like this

top left y x: k 35

after each sub window is input, you are asked if you are finished.

Once you have logged in you can use the following options:

1 Encode Text
Type in text and it will be encrypted with the key you currently have loaded.

2 Decode Text
Type in cipher text and it will be decrypted with the key you currently have loaded.

3 Encode File
enter a file name and the file will be encrypted with the key you currently have loaded.

4 Decode File
enter a file name and the file will be decrypted with the key you currently have loaded.

5 Load Key
enter a file name to load a key file.

6 Create Key
search the mandelbrot set to find a sub window to use as a key.

7 Enter Manual Key
enter the hex value for a key. This is your key in plain text so be careful with it.

8 Print Key
print the hex value of a key. This is your key in plain text so be careful with it.

0 Quit





