#include <asm-generic/errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

// Declariations
void processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
void processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
void processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
void processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
void processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                         int rotate_iteration);
void processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                        int rotate_iteration);
// Variable Declarations
unsigned char *rendered_frame_one;
unsigned char *rendered_frame_two;
bool scr_one = true;

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveDown(buffer_frame, width, height, offset * -1);
    }

    // allocate memory for temporary image buffer
    // unsigned char *rendered_frame = allocateFrame(width, height);
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

    // copy the temporary buffer back to original frame buffer
    // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    // deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
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
void processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveLeft(buffer_frame, width, height, offset * -1);
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


    // allocate memory for temporary image buffer
    // unsigned char *rendered_frame = allocateFrame(width, height);

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

    // copy the temporary buffer back to original frame buffer
    // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    // deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
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
void processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveUp(buffer_frame, width, height, offset * -1);
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

    // allocate memory for temporary image buffer
    // unsigned char *rendered_frame = allocateFrame(width, height);

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

    // copy the temporary buffer back to original frame buffer
    // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    // deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
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
void processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveRight(buffer_frame, width, height, offset * -1);
    }

    // allocate memory for temporary image buffer
    // unsigned char *rendered_frame = allocateFrame(width, height);
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

    // copy the temporary buffer back to original frame buffer
    // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    // deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
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
void processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                               int rotate_iteration) {
    // handle negative offsets
    if (rotate_iteration < 0){
        return processRotateCCW(buffer_frame, width, height, rotate_iteration * -1);
    }

    // allocate memory for temporary image buffer
    // unsigned char *rendered_frame = allocateFrame(width, height);

    unsigned char * scr_frame;
    unsigned char * dest_frame;

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
        // copy the temporary buffer back to original frame buffer
        // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);
    }

    // free temporary image buffer
    // deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer counter clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
void processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                int rotate_iteration) {
    if (rotate_iteration < 0){
        // handle negative offsets
        // rotating 90 degrees counter clockwise in opposite direction is equal to 90 degrees in cw direction
        for (int iteration = 0; iteration > rotate_iteration; iteration--) {
            processRotateCW(buffer_frame, width, height, 1);
        }
    } else {
        // rotating 90 degrees counter clockwise is equivalent of rotating 270 degrees clockwise
        for (int iteration = 0; iteration < rotate_iteration; iteration++) {
            processRotateCW(buffer_frame, width, height, 3);
        }
    }

    // return a pointer to the updated image buffer
    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
void processMirrorX(unsigned char *buffer_frame, unsigned int width, unsigned int height, int _unused) {
    // allocate memory for temporary image buffer
    // unsigned char *rendered_frame = allocateFrame(width, height);

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

    // copy the temporary buffer back to original frame buffer
    // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    // deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
void processMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int _unused) {
    // allocate memory for temporary image buffer
    // unsigned char *rendered_frame = allocateFrame(width, height);

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

    // copy the temporary buffer back to original frame buffer
    // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    // deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
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
    rendered_frame_one = copyFrame(frame_buffer, rendered_frame_one, width, height);
    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
//        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
//               sensor_values[sensorValueIdx].value);
        if (!strcmp(sensor_values[sensorValueIdx].key, "W")) {
            processMoveUp(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) {
            processMoveLeft(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) {
            processMoveDown(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {
            processMoveRight(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CW")) {
            processRotateCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) {
            processRotateCCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
            processMirrorX(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
            processMirrorY(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        }
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            if (scr_one)
                frame_buffer = copyFrame(rendered_frame_one, frame_buffer, width, height);
            else
                frame_buffer = copyFrame(rendered_frame_two, frame_buffer, width, height);
            verifyFrame(frame_buffer, width, height, grading_mode);
        }
    }
    deallocateFrame(rendered_frame_one);
    deallocateFrame(rendered_frame_two);
    return;
}