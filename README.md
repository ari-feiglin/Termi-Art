# **TERMI ART**

Termi Art is a pixel art pad for the terminal. All drawings can be saved to files, which can be viewed and edited later.  

## <u>**VERSION 0.0.1**</u>
Termi Art version 0.0.1 (v0.0.1) was released on November 29, 2020.

### <u>**DOWNLOADING AND USAGE:**</u>
To download, go into you terminal and navigate to the directory you want to download Termi Art into. Run git clone to download the repository, and navigate into it. Run make, and then run termi_art. Like so:

```
$ cd ~
$ git clone https://github.com/ari-feiglin/Termi-Art
$ cd Termi-Art
$ make
$ ./termi_art
```

Running termi_art without flags and arguments will cause it to give you an error, and a list of arguments you can pass it. Here it is:

```
USAGE: ./termi_art: <flag> [file path]

FLAGS:
-o: Open without editing
-e: Open with editing
-n: Create new
```

Pixel art created with termi art will be saved to a file that you specify. File extensions do not matter.

### <u>**ISSUES:**</u>
Termi Art files (tart files) are not compressed, so large pixel arts will take up a lot of memory. But I don't think this is too large an issue because making a large pixel art takes a while and is annoying, so this issue shouldn't affect anyone (I'll fix it though in the future)
