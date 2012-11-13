
define void @set_rounding_mode_up() nounwind uwtable {
  %1 = tail call i32 @fesetround(i32 2048) nounwind
  ret void
}

declare i32 @fesetround(i32) nounwind

define void @set_rounding_mode_down() nounwind uwtable {
  %1 = tail call i32 @fesetround(i32 1024) nounwind
  ret void
}

define { double, double } @f1(double %x_lo, double %x_hi) {
entry:
  %expt_even_are_both_pos_p = fcmp ogt double %x_lo, 0.000000e+00
  br i1 %expt_even_are_both_pos_p, label %expt_even_both_are_pos, label %expt_even_both_are_not_pos

expt_even_both_are_pos:                           ; preds = %entry
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x = fmul double %x_lo, %x_lo
  %expt_even_pos_block_res_lo_unroll_x1 = fmul double %expt_even_pos_block_res_lo_unroll_x, %expt_even_pos_block_res_lo_unroll_x
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x = fmul double %x_hi, %x_hi
  %expt_even_pos_block_res_hi_unroll_x2 = fmul double %expt_even_pos_block_res_hi_unroll_x, %expt_even_pos_block_res_hi_unroll_x
  br label %expt_even_merge

expt_even_both_are_not_pos:                       ; preds = %entry
  %expt_even_are_both_neg_p = fcmp olt double %x_hi, 0.000000e+00
  br i1 %expt_even_are_both_neg_p, label %expt_even_both_are_neg, label %expt_even_contains_zero

expt_even_both_are_neg:                           ; preds = %expt_even_both_are_not_pos
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x = fmul double %x_hi, %x_hi
  %expt_even_neg_block_res_lo_unroll_x3 = fmul double %expt_even_neg_block_res_lo_unroll_x, %expt_even_neg_block_res_lo_unroll_x
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x4 = fmul double %x_lo, %x_lo
  %expt_even_pos_block_res_hi_unroll_x5 = fmul double %expt_even_pos_block_res_hi_unroll_x4, %expt_even_pos_block_res_hi_unroll_x4
  br label %expt_even_merge

expt_even_contains_zero:                          ; preds = %expt_even_both_are_not_pos
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x = fmul double %x_lo, %x_lo
  %expt_even_mid_block_lo__unroll_x7 = fmul double %expt_even_mid_block_lo__unroll_x, %expt_even_mid_block_lo__unroll_x
  %expt_even_mid_block_hi__unroll_x = fmul double %x_hi, %x_hi
  %expt_even_mid_block_hi__unroll_x8 = fmul double %expt_even_mid_block_hi__unroll_x, %expt_even_mid_block_hi__unroll_x
  %expt_even_mid_block_res_hi_abtest = fcmp olt double %expt_even_mid_block_lo__unroll_x7, %expt_even_mid_block_hi__unroll_x8
  %expt_even_mid_block_res_hi_abmax = select i1 %expt_even_mid_block_res_hi_abtest, double %expt_even_mid_block_hi__unroll_x8, double %expt_even_mid_block_lo__unroll_x7
  br label %expt_even_merge

expt_even_merge:                                  ; preds = %expt_even_contains_zero, %expt_even_both_are_neg, %expt_even_both_are_pos
  %expt_lo = phi double [ %expt_even_pos_block_res_lo_unroll_x1, %expt_even_both_are_pos ], [ %expt_even_neg_block_res_lo_unroll_x3, %expt_even_both_are_neg ], [ 0.000000e+00, %expt_even_contains_zero ]
  %expt_hi = phi double [ %expt_even_pos_block_res_hi_unroll_x2, %expt_even_both_are_pos ], [ %expt_even_pos_block_res_hi_unroll_x5, %expt_even_both_are_neg ], [ %expt_even_mid_block_res_hi_abmax, %expt_even_contains_zero ]
  %f_res_interval_init = insertvalue { double, double } undef, double %expt_lo, 0
  %f_res_interval = insertvalue { double, double } %f_res_interval_init, double %expt_hi, 1
  ret { double, double } %f_res_interval
}