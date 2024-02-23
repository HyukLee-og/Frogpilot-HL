#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_189924777218598274);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_685719039652475410);
void car_H_mod_fun(double *state, double *out_7473975854780875689);
void car_f_fun(double *state, double dt, double *out_3618428039686707642);
void car_F_fun(double *state, double dt, double *out_3271948853494064664);
void car_h_25(double *state, double *unused, double *out_7083081804873248017);
void car_H_25(double *state, double *unused, double *out_5857214118221770732);
void car_h_24(double *state, double *unused, double *out_2993186067877070236);
void car_H_24(double *state, double *unused, double *out_3679999694614620759);
void car_h_30(double *state, double *unused, double *out_7240268473224377162);
void car_H_30(double *state, double *unused, double *out_3338881159714522105);
void car_h_26(double *state, double *unused, double *out_4203482421780044477);
void car_H_26(double *state, double *unused, double *out_8848026636613724660);
void car_h_27(double *state, double *unused, double *out_6625516219329489669);
void car_H_27(double *state, double *unused, double *out_1115287088530578888);
void car_h_29(double *state, double *unused, double *out_1780274939216099280);
void car_H_29(double *state, double *unused, double *out_2828649815400129921);
void car_h_28(double *state, double *unused, double *out_2985008981505940351);
void car_H_28(double *state, double *unused, double *out_7911048832469660495);
void car_h_31(double *state, double *unused, double *out_4963984974807651139);
void car_H_31(double *state, double *unused, double *out_5826568156344810304);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}