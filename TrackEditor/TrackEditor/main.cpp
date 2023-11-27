//
//  main.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 17/11/23.
//


#include "Editor.h"

int main(int argc, const char * argv[]) {
    Editor editor;

    while (!editor.getWindowShouldClose()) {
        editor.update();
        editor.render();
    }

    return 0;
}
