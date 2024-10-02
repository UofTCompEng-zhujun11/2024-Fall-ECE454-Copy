#include <asm-generic/errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

// Declariations
void processMoveUp(unsigned width, unsigned height, int offset);
void processMoveLeft(unsigned width, unsigned height, int offset);
void processMoveDown(unsigned width, unsigned height, int offset);
void processMoveRight(unsigned width, unsigned height, int offset);
void processRotateCW(unsigned width, unsigned height, int rotate_iteration, bool is_CCW);

// Variable Declarations
unsigned char *rendered_frame_one;
unsigned char *rendered_frame_two;
bool scr_one;

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveUp(unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveDown(width, height, offset * -1);
    }

    unsigned char * scr_frame;
    unsigned char * dest_frame;
    if (scr_one){
        scr_frame = rendered_frame_one;
        dest_frame = rendered_frame_two;
        scr_one = false;
    } else {
        scr_frame = rendered_frame_two;
        dest_frame = rendered_frame_one;
        scr_one = true;
    }

    // store shifted pixels to temporary buffer
    for (int row = 0; row < (height - offset); row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = (row + offset) * width * 3 + column * 3;
            dest_frame[position_rendered_frame] = scr_frame[position_buffer_frame];
            dest_frame[position_rendered_frame + 1] = scr_frame[position_buffer_frame + 1];
            dest_frame[position_rendered_frame + 2] = scr_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = (height - offset); row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            dest_frame[position_rendered_frame] = 255;
            dest_frame[position_rendered_frame + 1] = 255;
            dest_frame[position_rendered_frame + 2] = 255;
        }
    }

    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image left
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveRight(unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveLeft(width, height, offset * -1);
    }
    unsigned char * scr_frame;
    unsigned char * dest_frame;
    if (scr_one){
        scr_frame = rendered_frame_one;
        dest_frame = rendered_frame_two;
        scr_one = false;
    } else {
        scr_frame = rendered_frame_two;
        dest_frame = rendered_frame_one;
        scr_one = true;
    }

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = offset; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = row * width * 3 + (column - offset) * 3;
            dest_frame[position_rendered_frame] = scr_frame[position_buffer_frame];
            dest_frame[position_rendered_frame + 1] = scr_frame[position_buffer_frame + 1];
            dest_frame[position_rendered_frame + 2] = scr_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < offset; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            dest_frame[position_rendered_frame] = 255;
            dest_frame[position_rendered_frame + 1] = 255;
            dest_frame[position_rendered_frame + 2] = 255;
        }
    }

    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveDown(unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveUp(width, height, offset * -1);
    }
    unsigned char * scr_frame;
    unsigned char * dest_frame;
    if (scr_one){
        scr_frame = rendered_frame_one;
        dest_frame = rendered_frame_two;
        scr_one = false;
    } else {
        scr_frame = rendered_frame_two;
        dest_frame = rendered_frame_one;
        scr_one = true;
    }

    // store shifted pixels to temporary buffer
    for (int row = offset; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = (row - offset) * width * 3 + column * 3;
            dest_frame[position_rendered_frame] = scr_frame[position_buffer_frame];
            dest_frame[position_rendered_frame + 1] = scr_frame[position_buffer_frame + 1];
            dest_frame[position_rendered_frame + 2] = scr_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = 0; row < offset; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            dest_frame[position_rendered_frame] = 255;
            dest_frame[position_rendered_frame + 1] = 255;
            dest_frame[position_rendered_frame + 2] = 255;
        }
    }

    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveLeft(unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveRight(width, height, offset * -1);
    }

    unsigned char * scr_frame;
    unsigned char * dest_frame;
    if (scr_one){
        scr_frame = rendered_frame_one;
        dest_frame = rendered_frame_two;
        scr_one = false;
    } else {
        scr_frame = rendered_frame_two;
        dest_frame = rendered_frame_one;
        scr_one = true;
    }


    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < (width - offset); column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = row * width * 3 + (column + offset) * 3;
            dest_frame[position_rendered_frame] = scr_frame[position_buffer_frame];
            dest_frame[position_rendered_frame + 1] = scr_frame[position_buffer_frame + 1];
            dest_frame[position_rendered_frame + 2] = scr_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = 0; row < height; row++) {
        for (int column = width - offset; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            dest_frame[position_rendered_frame] = 255;
            dest_frame[position_rendered_frame + 1] = 255;
            dest_frame[position_rendered_frame + 2] = 255;
        }
    }

    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
void processRotateCW(unsigned width, unsigned height, int rotate_iteration, bool is_CCW) {

    rotate_iteration = rotate_iteration % 4;
    if (!rotate_iteration)
        return;

    unsigned char * scr_frame;
    unsigned char * dest_frame;

    if (is_CCW){
        switch (rotate_iteration) {
            case 1:
            case -3:
                rotate_iteration = 3;
                break;
            case 2:
            case -2:
                rotate_iteration = 2;
                break;
            case 3:
            case -1:
                rotate_iteration = 1;
                break;
        }
    } else {
        switch (rotate_iteration) {
            case 1:
            case -3:
                rotate_iteration = 1;
                break;
            case 2:
            case -2:
                rotate_iteration = 2;
                break;
            case 3:
            case -1:
                rotate_iteration = 3;
                break;
        }
    }

    // store shifted pixels to temporary buffer
    for (int iteration = 0; iteration < rotate_iteration; iteration++) {
        int render_column = width - 1;
        int render_row = 0;
        if (scr_one){
            scr_frame = rendered_frame_one;
            dest_frame = rendered_frame_two;
            scr_one = false;
        } else {
            scr_frame = rendered_frame_two;
            dest_frame = rendered_frame_one;
            scr_one = true;
        }
        for (int row = 0; row < width; row++) {
            for (int column = 0; column < height; column++) {
                int position_frame_buffer = row * width * 3 + column * 3;
                dest_frame[render_row * width * 3 + render_column * 3] = scr_frame[position_frame_buffer];
                dest_frame[render_row * width * 3 + render_column * 3 + 1] = scr_frame[position_frame_buffer + 1];
                dest_frame[render_row * width * 3 + render_column * 3 + 2] = scr_frame[position_frame_buffer + 2];
                render_row += 1;
            }
            render_row = 0;
            render_column -= 1;
        }
    }

    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
void processMirrorX(unsigned int width, unsigned int height, int _unused) {

    unsigned char * scr_frame;
    unsigned char * dest_frame;
    if (scr_one){
        scr_frame = rendered_frame_one;
        dest_frame = rendered_frame_two;
        scr_one = false;
    } else {
        scr_frame = rendered_frame_two;
        dest_frame = rendered_frame_one;
        scr_one = true;
    }

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * height * 3 + column * 3;
            int position_buffer_frame = (height - row - 1) * height * 3 + column * 3;
            dest_frame[position_rendered_frame] = scr_frame[position_buffer_frame];
            dest_frame[position_rendered_frame + 1] = scr_frame[position_buffer_frame + 1];
            dest_frame[position_rendered_frame + 2] = scr_frame[position_buffer_frame + 2];
        }
    }

    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
void processMirrorY(unsigned width, unsigned height, int _unused) {

    unsigned char * scr_frame;
    unsigned char * dest_frame;
    if (scr_one){
        scr_frame = rendered_frame_one;
        dest_frame = rendered_frame_two;
        scr_one = false;
    } else {
        scr_frame = rendered_frame_two;
        dest_frame = rendered_frame_one;
        scr_one = true;
    }

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * height * 3 + column * 3;
            int position_buffer_frame = row * height * 3 + (width - column - 1) * 3;
            dest_frame[position_rendered_frame] = scr_frame[position_buffer_frame];
            dest_frame[position_rendered_frame + 1] = scr_frame[position_buffer_frame + 1];
            dest_frame[position_rendered_frame + 2] = scr_frame[position_buffer_frame + 2];
        }
    }

    return;
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "default-name";

    // Please fill in your information
    char student_first_name[] = "john";
    char student_last_name[] = "doe";
    char student_student_number[] = "0000000000";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent_first_name: %s\n", student_first_name);
    printf("\tstudent_last_name: %s\n", student_last_name);
    printf("\tstudent_student_number: %s\n", student_student_number);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          You can modify anything else in this file
 ***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported  24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    int processed_frames = 0;
    rendered_frame_one = allocateFrame(width, height);
    rendered_frame_two = allocateFrame(width, height);
    memcpy(rendered_frame_one, frame_buffer, (width * height * 3) * sizeof(char));
    scr_one = true;
    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
//        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
//               sensor_values[sensorValueIdx].value);
        if (!strcmp(sensor_values[sensorValueIdx].key, "W")) {
            processMoveUp(width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) {
            processMoveLeft(width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) {
            processMoveDown(width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {
            processMoveRight(width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CW")) {
            processRotateCW(width, height, sensor_values[sensorValueIdx].value, false);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) {
            processRotateCW(width, height, sensor_values[sensorValueIdx].value, true);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
            processMirrorX(width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
            processMirrorY(width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        }
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            if (scr_one)
                memcpy(frame_buffer, rendered_frame_one, (width * height * 3) * sizeof(char));
            else
                memcpy(frame_buffer, rendered_frame_two, (width * height * 3) * sizeof(char));
            verifyFrame(frame_buffer, width, height, grading_mode);
        }
    }
    deallocateFrame(rendered_frame_one);
    deallocateFrame(rendered_frame_two);
    return;
}