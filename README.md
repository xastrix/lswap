![Logo](https://github.com/xastrix/lswap/blob/master/media/logo.png)  
This is a command line tool designed for swift translation between languages by simply copying and pasting data from the clipboard
```
lswap r/run --log
      With logging (Displaying nontranslated and translated results)

lswap c/config <SourceLanguage> <TargetLanguage>
      Change the source and target languages in the configuration file
```
## Showcase
#### RU > EN
![Showcase](https://github.com/xastrix/lswap/blob/master/media/showcase.gif)  
> Set configuration (RN > EN) - ```lswap config ru en```
#### EN > DE
![Showcase1](https://github.com/xastrix/lswap/blob/master/media/showcase1.gif)  
> Set configuration (EN > DE) - ```lswap config en de```
#### RU > UK
![Showcase2](https://github.com/xastrix/lswap/blob/master/media/showcase2.gif)  
> Set configuration (RU > UK) - ```lswap config ru uk```
## Compiling
Clone the repository
```c
 git clone --recursive https://github.com/xastrix/lswap.git
```
Launch ```build-vsXX.bat``` (Where XX is the version of your VS).  
After building, run ```lswap.sln``` (Visual Studio Solution File) and compile project by pressing ```CTRL + Shift + B```
### Requirements
* Visual Studio
* Windows SDK
* [premake5](https://github.com/premake/premake-core/releases)
## Dependencies
[curl](https://github.com/curl/curl)  
## License
Code protected by [MIT LICENSE](https://github.com/xastrix/lswap/blob/master/LICENSE)
