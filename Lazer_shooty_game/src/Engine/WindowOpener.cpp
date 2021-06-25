#include "WindowOpener.h"
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
WindowOpener::WindowOpener()
{
}

WindowOpener::~WindowOpener()
{
}

std::string WindowOpener::PrintAndOpenStuff()
{
    std::string returnString= "CANCELED";
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog("hanimclip", "($ProjectDir)", &outPath);

    if (result == NFD_OKAY) {
        puts("Success!");
        returnString = outPath;
        puts(outPath);
        free(outPath);
        return returnString;
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
        return returnString;
    }
    else {
        printf("Error: %s\n", NFD_GetError());
        throw(std::exception("Error!"));
    }

}

std::string WindowOpener::PrintAndOpenPng()
{
    std::string returnString = "CANCELED";
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog("png", "($ProjectDir)", &outPath);

    if (result == NFD_OKAY) {
        puts("Success!");
        returnString = outPath;
        puts(outPath);
        free(outPath);
        return returnString;
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
        return returnString;
    }
    else {
        printf("Error: %s\n", NFD_GetError());
        throw(std::exception("Error!"));
    }
}

std::string WindowOpener::SaveFileStuff()
{

    std::string returnString= "thisisnotasavelocation";
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_SaveDialog(NULL, "($ProjectDir)", &outPath);

    if (result == NFD_OKAY) {
        puts("Success!");
        returnString = outPath;
        puts(outPath);
        free(outPath);
        return returnString;
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
        return returnString;
    }
    else {
        printf("Error: %s\n", NFD_GetError());
        throw(std::exception("Error!"));
    }



    return returnString;
}

std::string WindowOpener::GetFilePathSave(const char* _extension)
{
    std::string returnString = "thisisnotasavelocation";
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_SaveDialog(_extension, "($ProjectDir)", &outPath);

    if (result == NFD_OKAY) {
        puts("Success!");
        returnString = outPath;
        puts(outPath);
        free(outPath);
        return returnString;
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
        return returnString;
    }
    else {
        printf("Error: %s\n", NFD_GetError());
        throw(std::exception("Error!"));
    }
}

std::string WindowOpener::GetFilepathOpen(const char* _extension)
{
    std::string returnString = "CANCELED";
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(_extension, "($ProjectDir)", &outPath);

    if (result == NFD_OKAY) {
        puts("Success!");
        returnString = outPath;
        puts(outPath);
        free(outPath);
        return returnString;
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
        return returnString;
    }
    else {
        printf("Error: %s\n", NFD_GetError());
        throw(std::exception("Error!"));
    }
}
