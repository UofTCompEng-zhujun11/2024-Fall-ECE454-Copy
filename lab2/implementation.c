#include <asm-generic/errno.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

//#defines
#define W 1
#define A 2
#define S 3
#define D 3
#define CW 4
#define CCW 5
#define MX 6
#define MY 7
#define OUTFRAME 25
#define noop 0b000001
#define CW1 0b000010
#define CW2 0b000100
#define CW3 0b001000
#define MX1 0b010000
#define MY1 0b100000


// Declariations
void processMoveUp(unsigned width, unsigned height, int offset);
void processMoveLeft(unsigned width, unsigned height, int offset);
void processRotateCW(unsigned width, unsigned height, int rotate_iteration);

// Variable Declarations
unsigned char *rendered_frame;
//vg: vertical translation or general
//h: horizontal translation
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

    if (offset > 0){
        memcpy(rendered_frame, rendered_frame + offset * width * 3, ((width - offset) * width * 3) * sizeof(char));
        memset(rendered_frame + (width - offset) * width * 3, 255, offset * width * 3 * sizeof(char));
    } else if (offset < 0) {
        offset *= -1;
        memmove(rendered_frame + offset * width * 3, rendered_frame, ((width - offset) * width * 3) * sizeof(char));
        memset(rendered_frame, 255, offset * width * 3 * sizeof(char));
    } else {
        return;
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

    if (offset > 0) {
        for (int row = 0; row < width; row++){
            memcpy(rendered_frame + row * width * 3, rendered_frame + row * width * 3 + offset * 3, (width - offset) * 3 * sizeof(char));
            memset(rendered_frame + row * width * 3 + (width - offset) * 3, 255, offset * 3 * sizeof(char));
        }
    } else if (offset < 0) {
        offset *= -1;
        for (int row = 0; row < width; row++){
            memmove(rendered_frame + row * width * 3 + offset * 3, rendered_frame + row * width * 3, (width - offset) * 3 * sizeof(char));
            memset(rendered_frame + row * width * 3, 255, offset * 3 * sizeof(char));
        }
    } else {
        return;
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
void processRotateCW(unsigned width, unsigned height, int rotate_iteration) {
    rotate_iteration = rotate_iteration % 4;
    if (!rotate_iteration)
        return;

    int total_pixal = width * width;
    int half_pixals = total_pixal / 2;
    int row, col;
    int scr_idx, dest_idx;
    int array_opposite_idx;
    char temp[3];

    // store shifted pixels to temporary buffer
    switch (rotate_iteration) {
        case 3:
            // Transpose the matrix
            for (row = 0; row < width; ++row) {
                for (col = row + 1; col < width; ++col) {
                    dest_idx = row * width * 3 + col * 3;
                    scr_idx = col * width * 3 + row * 3;
                    memcpy(temp, rendered_frame + dest_idx, 3 * sizeof(char));
                    memcpy(rendered_frame + dest_idx, rendered_frame + scr_idx, 3 * sizeof(char));
                    memcpy(rendered_frame + scr_idx, temp, 3 * sizeof(char));
                }
            }

            // Reverse each row
            for (col = 0; col < width; ++col) {
                for (row = 0; row < width / 2; ++row) {
                    dest_idx = row * width * 3 + col * 3;
                    scr_idx = (width - row - 1) * width * 3 + col * 3;
                    memcpy(temp, rendered_frame + dest_idx, 3 * sizeof(char));
                    memcpy(rendered_frame + dest_idx, rendered_frame + scr_idx, 3 * sizeof(char));
                    memcpy(rendered_frame + scr_idx, temp, 3 * sizeof(char));
                }
            }
            break;
        case 1:
            // Transpose the matrix
            for (row = 0; row < width; ++row) {
                for (col = row + 1; col < width; ++col) {
                    dest_idx = row * width * 3 + col * 3;
                    scr_idx = col * width * 3 + row * 3;
                    memcpy(temp, rendered_frame + dest_idx, 3 * sizeof(char));
                    memcpy(rendered_frame + dest_idx, rendered_frame + scr_idx, 3 * sizeof(char));
                    memcpy(rendered_frame + scr_idx, temp, 3 * sizeof(char));
                }
            }

            // Reverse each row
            for (row = 0; row < width; ++row) {
                for (col = 0; col < width / 2; ++col) {
                    dest_idx = row * width * 3 + col * 3;
                    scr_idx = row * width * 3 + (width - col - 1) * 3;
                    memcpy(temp, rendered_frame + dest_idx, 3 * sizeof(char));
                    memcpy(rendered_frame + dest_idx, rendered_frame + scr_idx, 3 * sizeof(char));
                    memcpy(rendered_frame + scr_idx, temp, 3 * sizeof(char));
                }
            }
            break;

        case 2:
            for (row = 0; row < half_pixals; ++row) {
                array_opposite_idx = (total_pixal - row - 1) * 3;
                memcpy(temp, rendered_frame + row * 3, 3 * sizeof(char));
                memcpy(rendered_frame + row * 3, rendered_frame + array_opposite_idx, 3 * sizeof(char));
                memcpy(rendered_frame + array_opposite_idx, temp, 3 * sizeof(char));
            }
            break;
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
void processMirrorX(unsigned int width, unsigned int height, int mirror_iteration) {

    if (mirror_iteration % 2 == 0)
        return;

    char* temp[10000];
    int dest_idx, scr_idx;
    int row_len = width * 3;
    // store shifted pixels to temporary buffer
    for (int row = 0; row < width / 2; row++) {
        dest_idx = row * width * 3;
        scr_idx = (width - row - 1) * width *3;
        memcpy(temp, rendered_frame + dest_idx, row_len *sizeof(char));
        memcpy(rendered_frame + dest_idx, rendered_frame + scr_idx, row_len * sizeof(char));
        memcpy(rendered_frame + scr_idx, temp, row_len * sizeof(char));
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
void processMirrorY(unsigned width, unsigned height, int mirror_iteration) {
    if (mirror_iteration % 2 == 0)
        return;

    char* temp[3];
    int scr_idx, dest_idx;

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width / 2; column++) {
            dest_idx = row * height * 3 + column * 3;
            scr_idx = row * height * 3 + (width - column - 1) * 3;
            memcpy(temp, rendered_frame + dest_idx, 3 * sizeof(char));
            memcpy(rendered_frame + dest_idx, rendered_frame + scr_idx, 3 * sizeof(char));
            memcpy(rendered_frame + scr_idx, temp, 3 * sizeof(char));
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

void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    int output_end_frame;
    int sensor_val_idx = 0;
    u_int8_t W_idx, A_idx, S_idx, D_idx, R_idx, MX_idx, MY_idx;
    u_int8_t CW_modifier;
    u_int8_t temp;
    int rotation_it, mirror_it;
    int local_rotate_it, local_mirrorX_it, local_mirrorY_it;
    u_int8_t modification_flags; //0b000001 = CW0, 0b000010 = CW1, 0b000100 = CW2, 0b001000 = CW3, 0b010000 = MX, 0b100000 = MY
    bool traking_rotate, tracking_mirrorX, tracking_mirrorY, update_modifier;
    int processed_kv_list[7];

    rendered_frame = allocateFrame(width, height);
    memcpy(rendered_frame, frame_buffer, (width * height * 3) * sizeof(char));

    while (sensor_val_idx < sensor_values_count) {
        W_idx = 0;
        A_idx = 1;
        S_idx = 2;
        D_idx = 3;
        R_idx = 4;
        MX_idx = 5;
        MY_idx = 6;
        CW_modifier = 1; //3 = invert or 1 = normal
        modification_flags = 0b000000;
        tracking_mirrorX = false;
        tracking_mirrorY = false;
        traking_rotate = false;
        update_modifier = false;
        local_mirrorX_it = 0;
        local_mirrorY_it = 0;
        local_rotate_it = 0;
        memset(processed_kv_list, 0, 7 * sizeof(int));
        output_end_frame = OUTFRAME + sensor_val_idx;

        for (; sensor_val_idx < output_end_frame && sensor_val_idx < sensor_values_count; sensor_val_idx++){
            // printf("in for loop %d\n", sensor_val_idx);
            if (!strcmp(sensor_values[sensor_val_idx].key, "W")) {
                // printf("W triggered\n");
                processed_kv_list[W_idx] += sensor_values[sensor_val_idx].value;

            } else if (!strcmp(sensor_values[sensor_val_idx].key, "A")) {
                // printf("A triggered\n");
                processed_kv_list[A_idx] += sensor_values[sensor_val_idx].value;

            } else if (!strcmp(sensor_values[sensor_val_idx].key, "S")) {
                processed_kv_list[S_idx] += sensor_values[sensor_val_idx].value;

            } else if (!strcmp(sensor_values[sensor_val_idx].key, "D")) {
                processed_kv_list[D_idx] += sensor_values[sensor_val_idx].value;

            } else if (!strcmp(sensor_values[sensor_val_idx].key, "CW")) {
                local_rotate_it += sensor_values[sensor_val_idx].value;
                traking_rotate = true;

            } else if (!strcmp(sensor_values[sensor_val_idx].key, "CCW")) {
                local_rotate_it -= sensor_values[sensor_val_idx].value;
                traking_rotate = true;

            } else if (!strcmp(sensor_values[sensor_val_idx].key, "MX")) {
                // printf("MX triggerd once\n");
                local_mirrorX_it += 1;
                tracking_mirrorX = true;

            } else if (!strcmp(sensor_values[sensor_val_idx].key, "MY")) {
                // printf("MY triggerd once\n");
                local_mirrorY_it += 1;
                tracking_mirrorY = true;
            }
            if (sensor_val_idx < sensor_values_count - 1){
                if (traking_rotate && ((strcmp(sensor_values[sensor_val_idx + 1].key, "CW") != 0 
                                   && strcmp(sensor_values[sensor_val_idx + 1].key, "CCW") != 0)
                                   || sensor_val_idx == output_end_frame - 1)){
                    rotation_it = ((local_rotate_it % 4 + 4) * CW_modifier) % 4;
                    // printf("local effective CW: %d, CW_modifier = %d\n", rotation_it, CW_modifier);
                    processed_kv_list[R_idx] += rotation_it;
                    modification_flags = (0b1 << (local_rotate_it % 4 + 4) % 4);
                    local_rotate_it = 0;
                    traking_rotate = false;
                    update_modifier = true;

                } else if (tracking_mirrorX && (strcmp(sensor_values[sensor_val_idx + 1].key, "MX") != 0 
                                            || sensor_val_idx == output_end_frame - 1)){
                    mirror_it = local_mirrorX_it % 2;
                    processed_kv_list[MX_idx] += mirror_it;
                    modification_flags = (mirror_it == 0) ? noop : MX1;
                    local_mirrorX_it = 0;
                    tracking_mirrorX = false;
                    update_modifier = true;

                } else if (tracking_mirrorY && (strcmp(sensor_values[sensor_val_idx + 1].key, "MY") != 0 
                                            || sensor_val_idx == output_end_frame - 1)){
                    mirror_it = local_mirrorY_it % 2;
                    processed_kv_list[MY_idx] += mirror_it;
                    modification_flags = (mirror_it == 0) ? noop : MY1;
                    local_mirrorY_it = 0;
                    tracking_mirrorY = false;
                    update_modifier = true;
                }
            } else {
                if (traking_rotate){
                    rotation_it = ((local_rotate_it % 4 + 4) * CW_modifier) % 4;
                    processed_kv_list[R_idx] += rotation_it;
                } else if (tracking_mirrorX)
                    processed_kv_list[MX_idx] += local_mirrorX_it;
                else if (tracking_mirrorY)
                    processed_kv_list[MY_idx] += local_mirrorY_it;
            }
            if (update_modifier){
                switch (modification_flags) {
                    case noop:
                        break;
                    case CW1:
                        temp = W_idx;
                        W_idx = A_idx;
                        A_idx = S_idx;
                        S_idx = D_idx;
                        D_idx = temp;
                        break;
                    case CW2:
                        temp = A_idx;
                        A_idx = D_idx;
                        D_idx = temp;
                        temp = W_idx;
                        W_idx = S_idx;
                        S_idx = temp;
                        break;
                    case CW3:
                        temp = W_idx;
                        W_idx = D_idx;
                        D_idx = S_idx;
                        S_idx = A_idx;
                        A_idx = temp;
                        break;
                    case MX1:
                        CW_modifier = (CW_modifier == 1) ? 3 : 1;
                        temp = W_idx;
                        W_idx = S_idx;
                        S_idx = temp;
                        break;
                    case MY1:
                        CW_modifier = (CW_modifier == 1) ? 3 : 1;
                        temp = A_idx;
                        A_idx = D_idx;
                        D_idx = temp;
                        break;
                    default:
                        printf("Holy fuck mod = %d\n", modification_flags);
                        printf("fucked at %d\n", sensor_val_idx);
                }
                // printf("mod = %d\n", modification_flags);
                // printf("W_idx = %d, A_idx = %d, S_idx = %d, D_idx = %d \n", W_idx, A_idx, S_idx, D_idx);
                update_modifier = false;
                modification_flags = noop;
            }
            // printf("A is %d\n", processed_kv_list[1]);

        }
        processMoveUp(width, height, processed_kv_list[0] - processed_kv_list[2]);
        processMoveLeft(width, height, processed_kv_list[1] - processed_kv_list[3]);
        processRotateCW(width, height, processed_kv_list[4]);
        processMirrorX(width, height, processed_kv_list[5]);
        processMirrorY(width, height, processed_kv_list[6]);
        if (sensor_val_idx % OUTFRAME == 0) {
            // printf("sensor#: %d\n", sensor_val_idx);
            memcpy(frame_buffer, rendered_frame, (width * height * 3) * sizeof(char));
            verifyFrame(frame_buffer, width, height, grading_mode);
        }
    }
    deallocateFrame(rendered_frame);
    return;
}