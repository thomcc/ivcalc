declare void @llvm.lifetime.start(i64, i8* nocapture) nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) nounwind
declare i32 @fesetround(i32) nounwind

define void @set_rounding_mode_up() nounwind uwtable {
  %1 = tail call i32 @fesetround(i32 2048) nounwind
  ret void
}

define void @set_rounding_mode_down() nounwind uwtable {
  %1 = tail call i32 @fesetround(i32 1024) nounwind
  ret void
}

define { double, double } @func1(double %a_lo, double %a_hi, double %aa_lo, double %aa_hi, double %b_lo, double %b_hi, double %c_lo, double %c_hi, double %d_lo, double %d_hi, double %e_lo, double %e_hi, double %f_lo, double %f_hi, double %g_lo, double %g_hi, double %h_lo, double %h_hi, double %i_lo, double %i_hi, double %j_lo, double %j_hi, double %k_lo, double %k_hi, double %l_lo, double %l_hi, double %m_lo, double %m_hi, double %n_lo, double %n_hi, double %o_lo, double %o_hi, double %p_lo, double %p_hi, double %q_lo, double %q_hi, double %r_lo, double %r_hi, double %s_lo, double %s_hi, double %t_lo, double %t_hi, double %u_lo, double %u_hi, double %v_lo, double %v_hi, double %w_lo, double %w_hi, double %x_lo, double %x_hi, double %y_lo, double %y_hi, double %z_lo, double %z_hi, double %za_lo, double %za_hi, double %zb_lo, double %zb_hi, double %zc_lo, double %zc_hi, double %zd_lo, double %zd_hi, double %ze_lo, double %ze_hi, double %zf_lo, double %zf_hi, double %zg_lo, double %zg_hi, double %zh_lo, double %zh_hi, double %zi_lo, double %zi_hi, double %zj_lo, double %zj_hi, double %zk_lo, double %zk_hi, double %zl_lo, double %zl_hi, double %zm_lo, double %zm_hi, double %zn_lo, double %zn_hi, double %zo_lo, double %zo_hi, double %zp_lo, double %zp_hi, double %zq_lo, double %zq_hi, double %zr_lo, double %zr_hi, double %zs_lo, double %zs_hi, double %zt_lo, double %zt_hi, double %zu_lo, double %zu_hi, double %zv_lo, double %zv_hi, double %zw_lo, double %zw_hi, double %zx_lo, double %zx_hi, double %zy_lo, double %zy_hi) {
entry:
  call void @set_rounding_mode_up()
  %expt_even_are_both_pos_p = fcmp ogt double %a_lo, 0.000000e+00
  br i1 %expt_even_are_both_pos_p, label %expt_even_both_are_pos, label %expt_even_both_are_not_pos

expt_even_both_are_pos:                           ; preds = %entry
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x = fmul double %a_lo, %a_lo

  %force_round_fr_mem = alloca double, align 8
  %0 = bitcast double* %force_round_fr_mem to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %0)
  store volatile double %expt_even_pos_block_res_lo_unroll_x, double* %force_round_fr_mem, align 8
  %force_roundfr_rounded = load volatile double* %force_round_fr_mem, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %0)

  %force_round_fr_mem7 = alloca double, align 8
  %1 = bitcast double* %force_round_fr_mem7 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %1)
  store volatile double %force_roundfr_rounded, double* %force_round_fr_mem7, align 8
  %force_roundfr_rounded8 = load volatile double* %force_round_fr_mem7, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %1)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x = fmul double %a_hi, %a_hi
  %force_round_fr_mem9 = alloca double, align 8
  %2 = bitcast double* %force_round_fr_mem9 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %2)
  store volatile double %expt_even_pos_block_res_hi_unroll_x, double* %force_round_fr_mem9, align 8
  %force_roundfr_rounded10 = load volatile double* %force_round_fr_mem9, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %2)
  %force_round_fr_mem11 = alloca double, align 8
  %3 = bitcast double* %force_round_fr_mem11 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %3)
  store volatile double %force_roundfr_rounded10, double* %force_round_fr_mem11, align 8
  %force_roundfr_rounded12 = load volatile double* %force_round_fr_mem11, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %3)
  br label %expt_even_merge

expt_even_both_are_not_pos:                       ; preds = %entry
  %expt_even_are_both_neg_p = fcmp olt double %a_hi, 0.000000e+00
  br i1 %expt_even_are_both_neg_p, label %expt_even_both_are_neg, label %expt_even_contains_zero

expt_even_both_are_neg:                           ; preds = %expt_even_both_are_not_pos
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x = fmul double %a_hi, %a_hi
  %force_round_fr_mem13 = alloca double, align 8
  %4 = bitcast double* %force_round_fr_mem13 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %4)
  store volatile double %expt_even_neg_block_res_lo_unroll_x, double* %force_round_fr_mem13, align 8
  %force_roundfr_rounded14 = load volatile double* %force_round_fr_mem13, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %4)
  %force_round_fr_mem15 = alloca double, align 8
  %5 = bitcast double* %force_round_fr_mem15 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %5)
  store volatile double %force_roundfr_rounded14, double* %force_round_fr_mem15, align 8
  %force_roundfr_rounded16 = load volatile double* %force_round_fr_mem15, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %5)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x17 = fmul double %a_lo, %a_lo
  %force_round_fr_mem18 = alloca double, align 8
  %6 = bitcast double* %force_round_fr_mem18 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %6)
  store volatile double %expt_even_pos_block_res_hi_unroll_x17, double* %force_round_fr_mem18, align 8
  %force_roundfr_rounded19 = load volatile double* %force_round_fr_mem18, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %6)
  %force_round_fr_mem21 = alloca double, align 8
  %7 = bitcast double* %force_round_fr_mem21 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %7)
  store volatile double %force_roundfr_rounded19, double* %force_round_fr_mem21, align 8
  %force_roundfr_rounded22 = load volatile double* %force_round_fr_mem21, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %7)
  br label %expt_even_merge

expt_even_contains_zero:                          ; preds = %expt_even_both_are_not_pos
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x = fmul double %a_lo, %a_lo
  %force_round_fr_mem23 = alloca double, align 8
  %8 = bitcast double* %force_round_fr_mem23 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %8)
  store volatile double %expt_even_mid_block_lo__unroll_x, double* %force_round_fr_mem23, align 8
  %force_roundfr_rounded24 = load volatile double* %force_round_fr_mem23, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %8)
  %force_round_fr_mem25 = alloca double, align 8
  %9 = bitcast double* %force_round_fr_mem25 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %9)
  store volatile double %force_roundfr_rounded24, double* %force_round_fr_mem25, align 8
  %force_roundfr_rounded26 = load volatile double* %force_round_fr_mem25, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %9)
  %expt_even_mid_block_hi__unroll_x = fmul double %a_hi, %a_hi
  %force_round_fr_mem27 = alloca double, align 8
  %10 = bitcast double* %force_round_fr_mem27 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %10)
  store volatile double %expt_even_mid_block_hi__unroll_x, double* %force_round_fr_mem27, align 8
  %force_roundfr_rounded28 = load volatile double* %force_round_fr_mem27, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %10)
  %force_round_fr_mem29 = alloca double, align 8
  %11 = bitcast double* %force_round_fr_mem29 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %11)
  store volatile double %force_roundfr_rounded28, double* %force_round_fr_mem29, align 8
  %force_roundfr_rounded30 = load volatile double* %force_round_fr_mem29, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %11)
  %expt_even_mid_block_res_hi_abtest = fcmp olt double %force_roundfr_rounded26, %force_roundfr_rounded30
  %expt_even_mid_block_res_hi_abmax = select i1 %expt_even_mid_block_res_hi_abtest, double %force_roundfr_rounded30, double %force_roundfr_rounded26
  br label %expt_even_merge

expt_even_merge:                                  ; preds = %expt_even_contains_zero, %expt_even_both_are_neg, %expt_even_both_are_pos
  %expt_lo = phi double [ %force_roundfr_rounded8, %expt_even_both_are_pos ], [ %force_roundfr_rounded16, %expt_even_both_are_neg ], [ 0.000000e+00, %expt_even_contains_zero ]
  %expt_hi = phi double [ %force_roundfr_rounded12, %expt_even_both_are_pos ], [ %force_roundfr_rounded22, %expt_even_both_are_neg ], [ %expt_even_mid_block_res_hi_abmax, %expt_even_contains_zero ]
  call void @set_rounding_mode_down()
  call void @set_rounding_mode_up()
  %neg_lo31 = fsub double -0.000000e+00, %expt_hi
  %neg_hi32 = fsub double -0.000000e+00, %expt_lo
  %div_hi_a = fdiv double %neg_lo31, 3.000000e+01
  %force_round_fr_mem33 = alloca double, align 8
  %12 = bitcast double* %force_round_fr_mem33 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %12)
  store volatile double %div_hi_a, double* %force_round_fr_mem33, align 8
  %force_roundfr_rounded34 = load volatile double* %force_round_fr_mem33, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %12)
  %force_round_fr_mem35 = alloca double, align 8
  %13 = bitcast double* %force_round_fr_mem35 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %13)
  store volatile double %div_hi_a, double* %force_round_fr_mem35, align 8
  %force_roundfr_rounded36 = load volatile double* %force_round_fr_mem35, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %13)
  %div_hi_c = fdiv double %neg_hi32, 3.000000e+01
  %force_round_fr_mem37 = alloca double, align 8
  %14 = bitcast double* %force_round_fr_mem37 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %14)
  store volatile double %div_hi_c, double* %force_round_fr_mem37, align 8
  %force_roundfr_rounded38 = load volatile double* %force_round_fr_mem37, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %14)
  %force_round_fr_mem39 = alloca double, align 8
  %15 = bitcast double* %force_round_fr_mem39 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %15)
  store volatile double %div_hi_c, double* %force_round_fr_mem39, align 8
  %force_roundfr_rounded40 = load volatile double* %force_round_fr_mem39, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %15)
  %div_hi_ab_test = fcmp olt double %force_roundfr_rounded34, %force_roundfr_rounded36
  %div_hi_cd_test = fcmp olt double %force_roundfr_rounded38, %force_roundfr_rounded40
  %div_hi_ab_max = select i1 %div_hi_ab_test, double %force_roundfr_rounded36, double %force_roundfr_rounded34
  %div_hi_cd_max = select i1 %div_hi_cd_test, double %force_roundfr_rounded40, double %force_roundfr_rounded38
  %div_hi_abcd_test = fcmp olt double %div_hi_ab_max, %div_hi_cd_max
  %div_hi_abcd_max = select i1 %div_hi_abcd_test, double %div_hi_cd_max, double %div_hi_ab_max
  call void @set_rounding_mode_down()
  %force_round_fr_mem41 = alloca double, align 8
  %16 = bitcast double* %force_round_fr_mem41 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %16)
  store volatile double %div_hi_a, double* %force_round_fr_mem41, align 8
  %force_roundfr_rounded42 = load volatile double* %force_round_fr_mem41, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %16)
  %force_round_fr_mem43 = alloca double, align 8
  %17 = bitcast double* %force_round_fr_mem43 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %17)
  store volatile double %div_hi_a, double* %force_round_fr_mem43, align 8
  %force_roundfr_rounded44 = load volatile double* %force_round_fr_mem43, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %17)
  %force_round_fr_mem45 = alloca double, align 8
  %18 = bitcast double* %force_round_fr_mem45 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %18)
  store volatile double %div_hi_c, double* %force_round_fr_mem45, align 8
  %force_roundfr_rounded46 = load volatile double* %force_round_fr_mem45, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %18)
  %force_round_fr_mem47 = alloca double, align 8
  %19 = bitcast double* %force_round_fr_mem47 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %19)
  store volatile double %div_hi_c, double* %force_round_fr_mem47, align 8
  %force_roundfr_rounded48 = load volatile double* %force_round_fr_mem47, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %19)
  %div_lo_ab_test = fcmp olt double %force_roundfr_rounded44, %force_roundfr_rounded42
  %div_lo_cd_test = fcmp olt double %force_roundfr_rounded48, %force_roundfr_rounded46
  %div_lo_cd_min = select i1 %div_lo_cd_test, double %force_roundfr_rounded48, double %force_roundfr_rounded46
  %div_lo_ab_min = select i1 %div_lo_ab_test, double %force_roundfr_rounded44, double %force_roundfr_rounded42
  %div_lo_abcd_test = fcmp olt double %div_lo_cd_min, %div_lo_ab_min
  %div_lo_abcd_min = select i1 %div_lo_abcd_test, double %div_lo_cd_min, double %div_lo_ab_min
  %expt_even_are_both_pos_p51 = fcmp olt double %div_hi_abcd_max, -0.000000e+00
  call void @set_rounding_mode_up()
  br i1 %expt_even_are_both_pos_p51, label %expt_even_both_are_pos52, label %expt_even_both_are_not_pos65

expt_even_both_are_pos52:                         ; preds = %expt_even_merge
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x53 = fmul double %div_hi_abcd_max, %div_hi_abcd_max
  %force_round_fr_mem54 = alloca double, align 8
  %20 = bitcast double* %force_round_fr_mem54 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %20)
  store volatile double %expt_even_pos_block_res_lo_unroll_x53, double* %force_round_fr_mem54, align 8
  %force_roundfr_rounded55 = load volatile double* %force_round_fr_mem54, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %20)
  %force_round_fr_mem57 = alloca double, align 8
  %21 = bitcast double* %force_round_fr_mem57 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %21)
  store volatile double %force_roundfr_rounded55, double* %force_round_fr_mem57, align 8
  %force_roundfr_rounded58 = load volatile double* %force_round_fr_mem57, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %21)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x59 = fmul double %div_lo_abcd_min, %div_lo_abcd_min
  %force_round_fr_mem60 = alloca double, align 8
  %22 = bitcast double* %force_round_fr_mem60 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %22)
  store volatile double %expt_even_pos_block_res_hi_unroll_x59, double* %force_round_fr_mem60, align 8
  %force_roundfr_rounded61 = load volatile double* %force_round_fr_mem60, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %22)
  %force_round_fr_mem63 = alloca double, align 8
  %23 = bitcast double* %force_round_fr_mem63 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %23)
  store volatile double %force_roundfr_rounded61, double* %force_round_fr_mem63, align 8
  %force_roundfr_rounded64 = load volatile double* %force_round_fr_mem63, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %23)
  br label %expt_even_merge95

expt_even_both_are_not_pos65:                     ; preds = %expt_even_merge
  %expt_even_are_both_neg_p66 = fcmp ogt double %div_lo_abcd_min, -0.000000e+00
  br i1 %expt_even_are_both_neg_p66, label %expt_even_both_are_neg67, label %expt_even_contains_zero80

expt_even_both_are_neg67:                         ; preds = %expt_even_both_are_not_pos65
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x68 = fmul double %div_lo_abcd_min, %div_lo_abcd_min
  %force_round_fr_mem69 = alloca double, align 8
  %24 = bitcast double* %force_round_fr_mem69 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %24)
  store volatile double %expt_even_neg_block_res_lo_unroll_x68, double* %force_round_fr_mem69, align 8
  %force_roundfr_rounded70 = load volatile double* %force_round_fr_mem69, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %24)
  %force_round_fr_mem72 = alloca double, align 8
  %25 = bitcast double* %force_round_fr_mem72 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %25)
  store volatile double %force_roundfr_rounded70, double* %force_round_fr_mem72, align 8
  %force_roundfr_rounded73 = load volatile double* %force_round_fr_mem72, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %25)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x74 = fmul double %div_hi_abcd_max, %div_hi_abcd_max
  %force_round_fr_mem75 = alloca double, align 8
  %26 = bitcast double* %force_round_fr_mem75 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %26)
  store volatile double %expt_even_pos_block_res_hi_unroll_x74, double* %force_round_fr_mem75, align 8
  %force_roundfr_rounded76 = load volatile double* %force_round_fr_mem75, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %26)
  %force_round_fr_mem78 = alloca double, align 8
  %27 = bitcast double* %force_round_fr_mem78 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %27)
  store volatile double %force_roundfr_rounded76, double* %force_round_fr_mem78, align 8
  %force_roundfr_rounded79 = load volatile double* %force_round_fr_mem78, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %27)
  br label %expt_even_merge95

expt_even_contains_zero80:                        ; preds = %expt_even_both_are_not_pos65
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x81 = fmul double %div_hi_abcd_max, %div_hi_abcd_max
  %force_round_fr_mem82 = alloca double, align 8
  %28 = bitcast double* %force_round_fr_mem82 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %28)
  store volatile double %expt_even_mid_block_lo__unroll_x81, double* %force_round_fr_mem82, align 8
  %force_roundfr_rounded83 = load volatile double* %force_round_fr_mem82, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %28)
  %force_round_fr_mem85 = alloca double, align 8
  %29 = bitcast double* %force_round_fr_mem85 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %29)
  store volatile double %force_roundfr_rounded83, double* %force_round_fr_mem85, align 8
  %force_roundfr_rounded86 = load volatile double* %force_round_fr_mem85, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %29)
  %expt_even_mid_block_hi__unroll_x87 = fmul double %div_lo_abcd_min, %div_lo_abcd_min
  %force_round_fr_mem88 = alloca double, align 8
  %30 = bitcast double* %force_round_fr_mem88 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %30)
  store volatile double %expt_even_mid_block_hi__unroll_x87, double* %force_round_fr_mem88, align 8
  %force_roundfr_rounded89 = load volatile double* %force_round_fr_mem88, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %30)
  %force_round_fr_mem91 = alloca double, align 8
  %31 = bitcast double* %force_round_fr_mem91 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %31)
  store volatile double %force_roundfr_rounded89, double* %force_round_fr_mem91, align 8
  %force_roundfr_rounded92 = load volatile double* %force_round_fr_mem91, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %31)
  %expt_even_mid_block_res_hi_abtest93 = fcmp olt double %force_roundfr_rounded86, %force_roundfr_rounded92
  %expt_even_mid_block_res_hi_abmax94 = select i1 %expt_even_mid_block_res_hi_abtest93, double %force_roundfr_rounded92, double %force_roundfr_rounded86
  br label %expt_even_merge95

expt_even_merge95:                                ; preds = %expt_even_contains_zero80, %expt_even_both_are_neg67, %expt_even_both_are_pos52
  %expt_lo96 = phi double [ %force_roundfr_rounded58, %expt_even_both_are_pos52 ], [ %force_roundfr_rounded73, %expt_even_both_are_neg67 ], [ 0.000000e+00, %expt_even_contains_zero80 ]
  %expt_hi97 = phi double [ %force_roundfr_rounded64, %expt_even_both_are_pos52 ], [ %force_roundfr_rounded79, %expt_even_both_are_neg67 ], [ %expt_even_mid_block_res_hi_abmax94, %expt_even_contains_zero80 ]
  %expt_even_are_both_pos_p98 = fcmp ogt double %expt_lo96, 0.000000e+00
  br i1 %expt_even_are_both_pos_p98, label %expt_even_both_are_pos99, label %expt_even_both_are_not_pos112

expt_even_both_are_pos99:                         ; preds = %expt_even_merge95
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x100 = fmul double %expt_lo96, %expt_lo96
  %force_round_fr_mem101 = alloca double, align 8
  %32 = bitcast double* %force_round_fr_mem101 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %32)
  store volatile double %expt_even_pos_block_res_lo_unroll_x100, double* %force_round_fr_mem101, align 8
  %force_roundfr_rounded102 = load volatile double* %force_round_fr_mem101, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %32)
  %force_round_fr_mem104 = alloca double, align 8
  %33 = bitcast double* %force_round_fr_mem104 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %33)
  store volatile double %force_roundfr_rounded102, double* %force_round_fr_mem104, align 8
  %force_roundfr_rounded105 = load volatile double* %force_round_fr_mem104, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %33)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x106 = fmul double %expt_hi97, %expt_hi97
  %force_round_fr_mem107 = alloca double, align 8
  %34 = bitcast double* %force_round_fr_mem107 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %34)
  store volatile double %expt_even_pos_block_res_hi_unroll_x106, double* %force_round_fr_mem107, align 8
  %force_roundfr_rounded108 = load volatile double* %force_round_fr_mem107, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %34)
  %force_round_fr_mem110 = alloca double, align 8
  %35 = bitcast double* %force_round_fr_mem110 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %35)
  store volatile double %force_roundfr_rounded108, double* %force_round_fr_mem110, align 8
  %force_roundfr_rounded111 = load volatile double* %force_round_fr_mem110, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %35)
  br label %expt_even_merge142

expt_even_both_are_not_pos112:                    ; preds = %expt_even_merge95
  %expt_even_are_both_neg_p113 = fcmp olt double %expt_hi97, 0.000000e+00
  br i1 %expt_even_are_both_neg_p113, label %expt_even_both_are_neg114, label %expt_even_contains_zero127

expt_even_both_are_neg114:                        ; preds = %expt_even_both_are_not_pos112
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x115 = fmul double %expt_hi97, %expt_hi97
  %force_round_fr_mem116 = alloca double, align 8
  %36 = bitcast double* %force_round_fr_mem116 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %36)
  store volatile double %expt_even_neg_block_res_lo_unroll_x115, double* %force_round_fr_mem116, align 8
  %force_roundfr_rounded117 = load volatile double* %force_round_fr_mem116, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %36)
  %force_round_fr_mem119 = alloca double, align 8
  %37 = bitcast double* %force_round_fr_mem119 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %37)
  store volatile double %force_roundfr_rounded117, double* %force_round_fr_mem119, align 8
  %force_roundfr_rounded120 = load volatile double* %force_round_fr_mem119, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %37)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x121 = fmul double %expt_lo96, %expt_lo96
  %force_round_fr_mem122 = alloca double, align 8
  %38 = bitcast double* %force_round_fr_mem122 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %38)
  store volatile double %expt_even_pos_block_res_hi_unroll_x121, double* %force_round_fr_mem122, align 8
  %force_roundfr_rounded123 = load volatile double* %force_round_fr_mem122, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %38)
  %force_round_fr_mem125 = alloca double, align 8
  %39 = bitcast double* %force_round_fr_mem125 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %39)
  store volatile double %force_roundfr_rounded123, double* %force_round_fr_mem125, align 8
  %force_roundfr_rounded126 = load volatile double* %force_round_fr_mem125, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %39)
  br label %expt_even_merge142

expt_even_contains_zero127:                       ; preds = %expt_even_both_are_not_pos112
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x128 = fmul double %expt_lo96, %expt_lo96
  %force_round_fr_mem129 = alloca double, align 8
  %40 = bitcast double* %force_round_fr_mem129 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %40)
  store volatile double %expt_even_mid_block_lo__unroll_x128, double* %force_round_fr_mem129, align 8
  %force_roundfr_rounded130 = load volatile double* %force_round_fr_mem129, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %40)
  %force_round_fr_mem132 = alloca double, align 8
  %41 = bitcast double* %force_round_fr_mem132 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %41)
  store volatile double %force_roundfr_rounded130, double* %force_round_fr_mem132, align 8
  %force_roundfr_rounded133 = load volatile double* %force_round_fr_mem132, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %41)
  %expt_even_mid_block_hi__unroll_x134 = fmul double %expt_hi97, %expt_hi97
  %force_round_fr_mem135 = alloca double, align 8
  %42 = bitcast double* %force_round_fr_mem135 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %42)
  store volatile double %expt_even_mid_block_hi__unroll_x134, double* %force_round_fr_mem135, align 8
  %force_roundfr_rounded136 = load volatile double* %force_round_fr_mem135, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %42)
  %force_round_fr_mem138 = alloca double, align 8
  %43 = bitcast double* %force_round_fr_mem138 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %43)
  store volatile double %force_roundfr_rounded136, double* %force_round_fr_mem138, align 8
  %force_roundfr_rounded139 = load volatile double* %force_round_fr_mem138, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %43)
  %expt_even_mid_block_res_hi_abtest140 = fcmp olt double %force_roundfr_rounded133, %force_roundfr_rounded139
  %expt_even_mid_block_res_hi_abmax141 = select i1 %expt_even_mid_block_res_hi_abtest140, double %force_roundfr_rounded139, double %force_roundfr_rounded133
  br label %expt_even_merge142

expt_even_merge142:                               ; preds = %expt_even_contains_zero127, %expt_even_both_are_neg114, %expt_even_both_are_pos99
  %expt_lo143 = phi double [ %force_roundfr_rounded105, %expt_even_both_are_pos99 ], [ %force_roundfr_rounded120, %expt_even_both_are_neg114 ], [ 0.000000e+00, %expt_even_contains_zero127 ]
  %expt_hi144 = phi double [ %force_roundfr_rounded111, %expt_even_both_are_pos99 ], [ %force_roundfr_rounded126, %expt_even_both_are_neg114 ], [ %expt_even_mid_block_res_hi_abmax141, %expt_even_contains_zero127 ]
  call void @set_rounding_mode_down()
  call void @set_rounding_mode_up()
  call void @set_rounding_mode_down()
  %add_lo = fadd double %expt_lo143, -6.300000e+01
  %force_round_fr_mem147 = alloca double, align 8
  %44 = bitcast double* %force_round_fr_mem147 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %44)
  store volatile double %add_lo, double* %force_round_fr_mem147, align 8
  %force_roundfr_rounded148 = load volatile double* %force_round_fr_mem147, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %44)
  call void @set_rounding_mode_up()
  %add_hi = fadd double %expt_hi144, -6.300000e+01
  %force_round_fr_mem149 = alloca double, align 8
  %45 = bitcast double* %force_round_fr_mem149 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %45)
  store volatile double %add_hi, double* %force_round_fr_mem149, align 8
  %force_roundfr_rounded150 = load volatile double* %force_round_fr_mem149, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %45)
  %mul_hi_a = fmul double %force_roundfr_rounded148, %b_lo
  %force_round_fr_mem151 = alloca double, align 8
  %46 = bitcast double* %force_round_fr_mem151 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %46)
  store volatile double %mul_hi_a, double* %force_round_fr_mem151, align 8
  %force_roundfr_rounded152 = load volatile double* %force_round_fr_mem151, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %46)
  %mul_hi_b = fmul double %force_roundfr_rounded148, %b_hi
  %force_round_fr_mem153 = alloca double, align 8
  %47 = bitcast double* %force_round_fr_mem153 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %47)
  store volatile double %mul_hi_b, double* %force_round_fr_mem153, align 8
  %force_roundfr_rounded154 = load volatile double* %force_round_fr_mem153, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %47)
  %mul_hi_c = fmul double %force_roundfr_rounded150, %b_lo
  %force_round_fr_mem155 = alloca double, align 8
  %48 = bitcast double* %force_round_fr_mem155 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %48)
  store volatile double %mul_hi_c, double* %force_round_fr_mem155, align 8
  %force_roundfr_rounded156 = load volatile double* %force_round_fr_mem155, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %48)
  %mul_hi_d = fmul double %force_roundfr_rounded150, %b_hi
  %force_round_fr_mem157 = alloca double, align 8
  %49 = bitcast double* %force_round_fr_mem157 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %49)
  store volatile double %mul_hi_d, double* %force_round_fr_mem157, align 8
  %force_roundfr_rounded158 = load volatile double* %force_round_fr_mem157, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %49)
  %mul_hi_ab_test = fcmp olt double %force_roundfr_rounded152, %force_roundfr_rounded154
  %mul_hi_cd_test = fcmp olt double %force_roundfr_rounded156, %force_roundfr_rounded158
  %mul_hi_ab_max = select i1 %mul_hi_ab_test, double %force_roundfr_rounded154, double %force_roundfr_rounded152
  %mul_hi_cd_max = select i1 %mul_hi_cd_test, double %force_roundfr_rounded158, double %force_roundfr_rounded156
  %mul_hi_abcd_test = fcmp olt double %mul_hi_ab_max, %mul_hi_cd_max
  %mul_hi_abcd_max = select i1 %mul_hi_abcd_test, double %mul_hi_cd_max, double %mul_hi_ab_max
  call void @set_rounding_mode_down()
  %force_round_fr_mem159 = alloca double, align 8
  %50 = bitcast double* %force_round_fr_mem159 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %50)
  store volatile double %mul_hi_a, double* %force_round_fr_mem159, align 8
  %force_roundfr_rounded160 = load volatile double* %force_round_fr_mem159, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %50)
  %force_round_fr_mem161 = alloca double, align 8
  %51 = bitcast double* %force_round_fr_mem161 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %51)
  store volatile double %mul_hi_b, double* %force_round_fr_mem161, align 8
  %force_roundfr_rounded162 = load volatile double* %force_round_fr_mem161, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %51)
  %force_round_fr_mem163 = alloca double, align 8
  %52 = bitcast double* %force_round_fr_mem163 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %52)
  store volatile double %mul_hi_c, double* %force_round_fr_mem163, align 8
  %force_roundfr_rounded164 = load volatile double* %force_round_fr_mem163, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %52)
  %force_round_fr_mem165 = alloca double, align 8
  %53 = bitcast double* %force_round_fr_mem165 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %53)
  store volatile double %mul_hi_d, double* %force_round_fr_mem165, align 8
  %force_roundfr_rounded166 = load volatile double* %force_round_fr_mem165, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %53)
  %mul_lo_ab_test = fcmp olt double %force_roundfr_rounded162, %force_roundfr_rounded160
  %mul_lo_cd_test = fcmp olt double %force_roundfr_rounded166, %force_roundfr_rounded164
  %mul_lo_cd_min = select i1 %mul_lo_cd_test, double %force_roundfr_rounded166, double %force_roundfr_rounded164
  %mul_lo_ab_min = select i1 %mul_lo_ab_test, double %force_roundfr_rounded162, double %force_roundfr_rounded160
  %mul_lo_abcd_test = fcmp olt double %mul_lo_cd_min, %mul_lo_ab_min
  %mul_lo_abcd_min = select i1 %mul_lo_abcd_test, double %mul_lo_cd_min, double %mul_lo_ab_min
  %add_lo167 = fadd double %mul_lo_abcd_min, %c_lo
  %force_round_fr_mem168 = alloca double, align 8
  %54 = bitcast double* %force_round_fr_mem168 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %54)
  store volatile double %add_lo167, double* %force_round_fr_mem168, align 8
  %force_roundfr_rounded169 = load volatile double* %force_round_fr_mem168, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %54)
  call void @set_rounding_mode_up()
  %add_hi170 = fadd double %mul_hi_abcd_max, %c_hi
  %force_round_fr_mem171 = alloca double, align 8
  %55 = bitcast double* %force_round_fr_mem171 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %55)
  store volatile double %add_hi170, double* %force_round_fr_mem171, align 8
  %force_roundfr_rounded172 = load volatile double* %force_round_fr_mem171, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %55)
  %expt_even_are_both_pos_p173 = fcmp ogt double %force_roundfr_rounded169, 0.000000e+00
  br i1 %expt_even_are_both_pos_p173, label %expt_even_both_are_pos174, label %expt_even_both_are_not_pos187

expt_even_both_are_pos174:                        ; preds = %expt_even_merge142
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x175 = fmul double %force_roundfr_rounded169, %force_roundfr_rounded169
  %force_round_fr_mem176 = alloca double, align 8
  %56 = bitcast double* %force_round_fr_mem176 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %56)
  store volatile double %expt_even_pos_block_res_lo_unroll_x175, double* %force_round_fr_mem176, align 8
  %force_roundfr_rounded177 = load volatile double* %force_round_fr_mem176, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %56)
  %force_round_fr_mem179 = alloca double, align 8
  %57 = bitcast double* %force_round_fr_mem179 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %57)
  store volatile double %force_roundfr_rounded177, double* %force_round_fr_mem179, align 8
  %force_roundfr_rounded180 = load volatile double* %force_round_fr_mem179, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %57)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x181 = fmul double %force_roundfr_rounded172, %force_roundfr_rounded172
  %force_round_fr_mem182 = alloca double, align 8
  %58 = bitcast double* %force_round_fr_mem182 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %58)
  store volatile double %expt_even_pos_block_res_hi_unroll_x181, double* %force_round_fr_mem182, align 8
  %force_roundfr_rounded183 = load volatile double* %force_round_fr_mem182, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %58)
  %force_round_fr_mem185 = alloca double, align 8
  %59 = bitcast double* %force_round_fr_mem185 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %59)
  store volatile double %force_roundfr_rounded183, double* %force_round_fr_mem185, align 8
  %force_roundfr_rounded186 = load volatile double* %force_round_fr_mem185, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %59)
  br label %expt_even_merge217

expt_even_both_are_not_pos187:                    ; preds = %expt_even_merge142
  %expt_even_are_both_neg_p188 = fcmp olt double %force_roundfr_rounded172, 0.000000e+00
  br i1 %expt_even_are_both_neg_p188, label %expt_even_both_are_neg189, label %expt_even_contains_zero202

expt_even_both_are_neg189:                        ; preds = %expt_even_both_are_not_pos187
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x190 = fmul double %force_roundfr_rounded172, %force_roundfr_rounded172
  %force_round_fr_mem191 = alloca double, align 8
  %60 = bitcast double* %force_round_fr_mem191 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %60)
  store volatile double %expt_even_neg_block_res_lo_unroll_x190, double* %force_round_fr_mem191, align 8
  %force_roundfr_rounded192 = load volatile double* %force_round_fr_mem191, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %60)
  %force_round_fr_mem194 = alloca double, align 8
  %61 = bitcast double* %force_round_fr_mem194 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %61)
  store volatile double %force_roundfr_rounded192, double* %force_round_fr_mem194, align 8
  %force_roundfr_rounded195 = load volatile double* %force_round_fr_mem194, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %61)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x196 = fmul double %force_roundfr_rounded169, %force_roundfr_rounded169
  %force_round_fr_mem197 = alloca double, align 8
  %62 = bitcast double* %force_round_fr_mem197 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %62)
  store volatile double %expt_even_pos_block_res_hi_unroll_x196, double* %force_round_fr_mem197, align 8
  %force_roundfr_rounded198 = load volatile double* %force_round_fr_mem197, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %62)
  %force_round_fr_mem200 = alloca double, align 8
  %63 = bitcast double* %force_round_fr_mem200 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %63)
  store volatile double %force_roundfr_rounded198, double* %force_round_fr_mem200, align 8
  %force_roundfr_rounded201 = load volatile double* %force_round_fr_mem200, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %63)
  br label %expt_even_merge217

expt_even_contains_zero202:                       ; preds = %expt_even_both_are_not_pos187
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x203 = fmul double %force_roundfr_rounded169, %force_roundfr_rounded169
  %force_round_fr_mem204 = alloca double, align 8
  %64 = bitcast double* %force_round_fr_mem204 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %64)
  store volatile double %expt_even_mid_block_lo__unroll_x203, double* %force_round_fr_mem204, align 8
  %force_roundfr_rounded205 = load volatile double* %force_round_fr_mem204, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %64)
  %force_round_fr_mem207 = alloca double, align 8
  %65 = bitcast double* %force_round_fr_mem207 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %65)
  store volatile double %force_roundfr_rounded205, double* %force_round_fr_mem207, align 8
  %force_roundfr_rounded208 = load volatile double* %force_round_fr_mem207, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %65)
  %expt_even_mid_block_hi__unroll_x209 = fmul double %force_roundfr_rounded172, %force_roundfr_rounded172
  %force_round_fr_mem210 = alloca double, align 8
  %66 = bitcast double* %force_round_fr_mem210 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %66)
  store volatile double %expt_even_mid_block_hi__unroll_x209, double* %force_round_fr_mem210, align 8
  %force_roundfr_rounded211 = load volatile double* %force_round_fr_mem210, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %66)
  %force_round_fr_mem213 = alloca double, align 8
  %67 = bitcast double* %force_round_fr_mem213 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %67)
  store volatile double %force_roundfr_rounded211, double* %force_round_fr_mem213, align 8
  %force_roundfr_rounded214 = load volatile double* %force_round_fr_mem213, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %67)
  %expt_even_mid_block_res_hi_abtest215 = fcmp olt double %force_roundfr_rounded208, %force_roundfr_rounded214
  %expt_even_mid_block_res_hi_abmax216 = select i1 %expt_even_mid_block_res_hi_abtest215, double %force_roundfr_rounded214, double %force_roundfr_rounded208
  br label %expt_even_merge217

expt_even_merge217:                               ; preds = %expt_even_contains_zero202, %expt_even_both_are_neg189, %expt_even_both_are_pos174
  %expt_lo218 = phi double [ %force_roundfr_rounded180, %expt_even_both_are_pos174 ], [ %force_roundfr_rounded195, %expt_even_both_are_neg189 ], [ 0.000000e+00, %expt_even_contains_zero202 ]
  %expt_hi219 = phi double [ %force_roundfr_rounded186, %expt_even_both_are_pos174 ], [ %force_roundfr_rounded201, %expt_even_both_are_neg189 ], [ %expt_even_mid_block_res_hi_abmax216, %expt_even_contains_zero202 ]
  %div_hi_a220 = fdiv double %expt_lo218, %d_lo
  %force_round_fr_mem221 = alloca double, align 8
  %68 = bitcast double* %force_round_fr_mem221 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %68)
  store volatile double %div_hi_a220, double* %force_round_fr_mem221, align 8
  %force_roundfr_rounded222 = load volatile double* %force_round_fr_mem221, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %68)
  %div_hi_b223 = fdiv double %expt_lo218, %d_hi
  %force_round_fr_mem224 = alloca double, align 8
  %69 = bitcast double* %force_round_fr_mem224 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %69)
  store volatile double %div_hi_b223, double* %force_round_fr_mem224, align 8
  %force_roundfr_rounded225 = load volatile double* %force_round_fr_mem224, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %69)
  %div_hi_c226 = fdiv double %expt_hi219, %d_lo
  %force_round_fr_mem227 = alloca double, align 8
  %70 = bitcast double* %force_round_fr_mem227 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %70)
  store volatile double %div_hi_c226, double* %force_round_fr_mem227, align 8
  %force_roundfr_rounded228 = load volatile double* %force_round_fr_mem227, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %70)
  %div_hi_d229 = fdiv double %expt_hi219, %d_hi
  %force_round_fr_mem230 = alloca double, align 8
  %71 = bitcast double* %force_round_fr_mem230 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %71)
  store volatile double %div_hi_d229, double* %force_round_fr_mem230, align 8
  %force_roundfr_rounded231 = load volatile double* %force_round_fr_mem230, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %71)
  %div_hi_ab_test232 = fcmp olt double %force_roundfr_rounded222, %force_roundfr_rounded225
  %div_hi_cd_test233 = fcmp olt double %force_roundfr_rounded228, %force_roundfr_rounded231
  %div_hi_ab_max234 = select i1 %div_hi_ab_test232, double %force_roundfr_rounded225, double %force_roundfr_rounded222
  %div_hi_cd_max235 = select i1 %div_hi_cd_test233, double %force_roundfr_rounded231, double %force_roundfr_rounded228
  %div_hi_abcd_test236 = fcmp olt double %div_hi_ab_max234, %div_hi_cd_max235
  %div_hi_abcd_max237 = select i1 %div_hi_abcd_test236, double %div_hi_cd_max235, double %div_hi_ab_max234
  call void @set_rounding_mode_down()
  %force_round_fr_mem239 = alloca double, align 8
  %72 = bitcast double* %force_round_fr_mem239 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %72)
  store volatile double %div_hi_a220, double* %force_round_fr_mem239, align 8
  %force_roundfr_rounded240 = load volatile double* %force_round_fr_mem239, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %72)
  %force_round_fr_mem242 = alloca double, align 8
  %73 = bitcast double* %force_round_fr_mem242 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %73)
  store volatile double %div_hi_b223, double* %force_round_fr_mem242, align 8
  %force_roundfr_rounded243 = load volatile double* %force_round_fr_mem242, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %73)
  %force_round_fr_mem245 = alloca double, align 8
  %74 = bitcast double* %force_round_fr_mem245 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %74)
  store volatile double %div_hi_c226, double* %force_round_fr_mem245, align 8
  %force_roundfr_rounded246 = load volatile double* %force_round_fr_mem245, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %74)
  %force_round_fr_mem248 = alloca double, align 8
  %75 = bitcast double* %force_round_fr_mem248 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %75)
  store volatile double %div_hi_d229, double* %force_round_fr_mem248, align 8
  %force_roundfr_rounded249 = load volatile double* %force_round_fr_mem248, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %75)
  %div_lo_ab_test250 = fcmp olt double %force_roundfr_rounded243, %force_roundfr_rounded240
  %div_lo_cd_test251 = fcmp olt double %force_roundfr_rounded249, %force_roundfr_rounded246
  %div_lo_cd_min252 = select i1 %div_lo_cd_test251, double %force_roundfr_rounded249, double %force_roundfr_rounded246
  %div_lo_ab_min253 = select i1 %div_lo_ab_test250, double %force_roundfr_rounded243, double %force_roundfr_rounded240
  %div_lo_abcd_test254 = fcmp olt double %div_lo_cd_min252, %div_lo_ab_min253
  %div_lo_abcd_min255 = select i1 %div_lo_abcd_test254, double %div_lo_cd_min252, double %div_lo_ab_min253
  call void @set_rounding_mode_up()
  %mul_hi_a256 = fmul double %div_lo_abcd_min255, 4.900000e+01
  %force_round_fr_mem257 = alloca double, align 8
  %76 = bitcast double* %force_round_fr_mem257 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %76)
  store volatile double %mul_hi_a256, double* %force_round_fr_mem257, align 8
  %force_roundfr_rounded258 = load volatile double* %force_round_fr_mem257, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %76)
  %mul_hi_b259 = fmul double %div_hi_abcd_max237, 4.900000e+01
  %force_round_fr_mem260 = alloca double, align 8
  %77 = bitcast double* %force_round_fr_mem260 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %77)
  store volatile double %mul_hi_b259, double* %force_round_fr_mem260, align 8
  %force_roundfr_rounded261 = load volatile double* %force_round_fr_mem260, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %77)
  %force_round_fr_mem263 = alloca double, align 8
  %78 = bitcast double* %force_round_fr_mem263 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %78)
  store volatile double %mul_hi_a256, double* %force_round_fr_mem263, align 8
  %force_roundfr_rounded264 = load volatile double* %force_round_fr_mem263, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %78)
  %force_round_fr_mem266 = alloca double, align 8
  %79 = bitcast double* %force_round_fr_mem266 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %79)
  store volatile double %mul_hi_b259, double* %force_round_fr_mem266, align 8
  %force_roundfr_rounded267 = load volatile double* %force_round_fr_mem266, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %79)
  %mul_hi_ab_test268 = fcmp olt double %force_roundfr_rounded258, %force_roundfr_rounded261
  %mul_hi_cd_test269 = fcmp olt double %force_roundfr_rounded264, %force_roundfr_rounded267
  %mul_hi_ab_max270 = select i1 %mul_hi_ab_test268, double %force_roundfr_rounded261, double %force_roundfr_rounded258
  %mul_hi_cd_max271 = select i1 %mul_hi_cd_test269, double %force_roundfr_rounded267, double %force_roundfr_rounded264
  %mul_hi_abcd_test272 = fcmp olt double %mul_hi_ab_max270, %mul_hi_cd_max271
  %mul_hi_abcd_max273 = select i1 %mul_hi_abcd_test272, double %mul_hi_cd_max271, double %mul_hi_ab_max270
  call void @set_rounding_mode_down()
  %force_round_fr_mem275 = alloca double, align 8
  %80 = bitcast double* %force_round_fr_mem275 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %80)
  store volatile double %mul_hi_a256, double* %force_round_fr_mem275, align 8
  %force_roundfr_rounded276 = load volatile double* %force_round_fr_mem275, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %80)
  %force_round_fr_mem278 = alloca double, align 8
  %81 = bitcast double* %force_round_fr_mem278 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %81)
  store volatile double %mul_hi_b259, double* %force_round_fr_mem278, align 8
  %force_roundfr_rounded279 = load volatile double* %force_round_fr_mem278, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %81)
  %force_round_fr_mem281 = alloca double, align 8
  %82 = bitcast double* %force_round_fr_mem281 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %82)
  store volatile double %mul_hi_a256, double* %force_round_fr_mem281, align 8
  %force_roundfr_rounded282 = load volatile double* %force_round_fr_mem281, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %82)
  %force_round_fr_mem284 = alloca double, align 8
  %83 = bitcast double* %force_round_fr_mem284 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %83)
  store volatile double %mul_hi_b259, double* %force_round_fr_mem284, align 8
  %force_roundfr_rounded285 = load volatile double* %force_round_fr_mem284, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %83)
  %mul_lo_ab_test286 = fcmp olt double %force_roundfr_rounded279, %force_roundfr_rounded276
  %mul_lo_cd_test287 = fcmp olt double %force_roundfr_rounded285, %force_roundfr_rounded282
  %mul_lo_cd_min288 = select i1 %mul_lo_cd_test287, double %force_roundfr_rounded285, double %force_roundfr_rounded282
  %mul_lo_ab_min289 = select i1 %mul_lo_ab_test286, double %force_roundfr_rounded279, double %force_roundfr_rounded276
  %mul_lo_abcd_test290 = fcmp olt double %mul_lo_cd_min288, %mul_lo_ab_min289
  %mul_lo_abcd_min291 = select i1 %mul_lo_abcd_test290, double %mul_lo_cd_min288, double %mul_lo_ab_min289
  %expt_odd_lo_unroll_x = fmul double %mul_lo_abcd_min291, %mul_lo_abcd_min291
  %force_round_fr_mem292 = alloca double, align 8
  %84 = bitcast double* %force_round_fr_mem292 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %84)
  store volatile double %expt_odd_lo_unroll_x, double* %force_round_fr_mem292, align 8
  %force_roundfr_rounded293 = load volatile double* %force_round_fr_mem292, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %84)
  %expt_odd_lopwr_do_y = fmul double %force_roundfr_rounded293, %mul_lo_abcd_min291
  %force_round_fr_mem294 = alloca double, align 8
  %85 = bitcast double* %force_round_fr_mem294 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %85)
  store volatile double %expt_odd_lopwr_do_y, double* %force_round_fr_mem294, align 8
  %force_roundfr_rounded295 = load volatile double* %force_round_fr_mem294, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %85)
  call void @set_rounding_mode_up()
  %expt_odd_hi_unroll_x = fmul double %mul_hi_abcd_max273, %mul_hi_abcd_max273
  %force_round_fr_mem296 = alloca double, align 8
  %86 = bitcast double* %force_round_fr_mem296 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %86)
  store volatile double %expt_odd_hi_unroll_x, double* %force_round_fr_mem296, align 8
  %force_roundfr_rounded297 = load volatile double* %force_round_fr_mem296, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %86)
  %expt_odd_hipwr_do_y = fmul double %force_roundfr_rounded297, %mul_hi_abcd_max273
  %force_round_fr_mem298 = alloca double, align 8
  %87 = bitcast double* %force_round_fr_mem298 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %87)
  store volatile double %expt_odd_hipwr_do_y, double* %force_round_fr_mem298, align 8
  %force_roundfr_rounded299 = load volatile double* %force_round_fr_mem298, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %87)
  call void @set_rounding_mode_down()
  %add_lo302 = fsub double -6.700000e+01, %force_roundfr_rounded299
  %force_round_fr_mem303 = alloca double, align 8
  %88 = bitcast double* %force_round_fr_mem303 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %88)
  store volatile double %add_lo302, double* %force_round_fr_mem303, align 8
  %force_roundfr_rounded304 = load volatile double* %force_round_fr_mem303, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %88)
  call void @set_rounding_mode_up()
  %add_hi305 = fsub double -6.700000e+01, %force_roundfr_rounded295
  %force_round_fr_mem306 = alloca double, align 8
  %89 = bitcast double* %force_round_fr_mem306 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %89)
  store volatile double %add_hi305, double* %force_round_fr_mem306, align 8
  %force_roundfr_rounded307 = load volatile double* %force_round_fr_mem306, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %89)
  %expt_even_are_both_pos_p308 = fcmp ogt double %force_roundfr_rounded304, 0.000000e+00
  br i1 %expt_even_are_both_pos_p308, label %expt_even_both_are_pos309, label %expt_even_both_are_not_pos322

expt_even_both_are_pos309:                        ; preds = %expt_even_merge217
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x310 = fmul double %force_roundfr_rounded304, %force_roundfr_rounded304
  %force_round_fr_mem311 = alloca double, align 8
  %90 = bitcast double* %force_round_fr_mem311 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %90)
  store volatile double %expt_even_pos_block_res_lo_unroll_x310, double* %force_round_fr_mem311, align 8
  %force_roundfr_rounded312 = load volatile double* %force_round_fr_mem311, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %90)
  %force_round_fr_mem314 = alloca double, align 8
  %91 = bitcast double* %force_round_fr_mem314 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %91)
  store volatile double %force_roundfr_rounded312, double* %force_round_fr_mem314, align 8
  %force_roundfr_rounded315 = load volatile double* %force_round_fr_mem314, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %91)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x316 = fmul double %force_roundfr_rounded307, %force_roundfr_rounded307
  %force_round_fr_mem317 = alloca double, align 8
  %92 = bitcast double* %force_round_fr_mem317 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %92)
  store volatile double %expt_even_pos_block_res_hi_unroll_x316, double* %force_round_fr_mem317, align 8
  %force_roundfr_rounded318 = load volatile double* %force_round_fr_mem317, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %92)
  %force_round_fr_mem320 = alloca double, align 8
  %93 = bitcast double* %force_round_fr_mem320 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %93)
  store volatile double %force_roundfr_rounded318, double* %force_round_fr_mem320, align 8
  %force_roundfr_rounded321 = load volatile double* %force_round_fr_mem320, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %93)
  br label %expt_even_merge352

expt_even_both_are_not_pos322:                    ; preds = %expt_even_merge217
  %expt_even_are_both_neg_p323 = fcmp olt double %force_roundfr_rounded307, 0.000000e+00
  br i1 %expt_even_are_both_neg_p323, label %expt_even_both_are_neg324, label %expt_even_contains_zero337

expt_even_both_are_neg324:                        ; preds = %expt_even_both_are_not_pos322
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x325 = fmul double %force_roundfr_rounded307, %force_roundfr_rounded307
  %force_round_fr_mem326 = alloca double, align 8
  %94 = bitcast double* %force_round_fr_mem326 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %94)
  store volatile double %expt_even_neg_block_res_lo_unroll_x325, double* %force_round_fr_mem326, align 8
  %force_roundfr_rounded327 = load volatile double* %force_round_fr_mem326, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %94)
  %force_round_fr_mem329 = alloca double, align 8
  %95 = bitcast double* %force_round_fr_mem329 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %95)
  store volatile double %force_roundfr_rounded327, double* %force_round_fr_mem329, align 8
  %force_roundfr_rounded330 = load volatile double* %force_round_fr_mem329, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %95)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x331 = fmul double %force_roundfr_rounded304, %force_roundfr_rounded304
  %force_round_fr_mem332 = alloca double, align 8
  %96 = bitcast double* %force_round_fr_mem332 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %96)
  store volatile double %expt_even_pos_block_res_hi_unroll_x331, double* %force_round_fr_mem332, align 8
  %force_roundfr_rounded333 = load volatile double* %force_round_fr_mem332, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %96)
  %force_round_fr_mem335 = alloca double, align 8
  %97 = bitcast double* %force_round_fr_mem335 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %97)
  store volatile double %force_roundfr_rounded333, double* %force_round_fr_mem335, align 8
  %force_roundfr_rounded336 = load volatile double* %force_round_fr_mem335, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %97)
  br label %expt_even_merge352

expt_even_contains_zero337:                       ; preds = %expt_even_both_are_not_pos322
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x338 = fmul double %force_roundfr_rounded304, %force_roundfr_rounded304
  %force_round_fr_mem339 = alloca double, align 8
  %98 = bitcast double* %force_round_fr_mem339 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %98)
  store volatile double %expt_even_mid_block_lo__unroll_x338, double* %force_round_fr_mem339, align 8
  %force_roundfr_rounded340 = load volatile double* %force_round_fr_mem339, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %98)
  %force_round_fr_mem342 = alloca double, align 8
  %99 = bitcast double* %force_round_fr_mem342 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %99)
  store volatile double %force_roundfr_rounded340, double* %force_round_fr_mem342, align 8
  %force_roundfr_rounded343 = load volatile double* %force_round_fr_mem342, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %99)
  %expt_even_mid_block_hi__unroll_x344 = fmul double %force_roundfr_rounded307, %force_roundfr_rounded307
  %force_round_fr_mem345 = alloca double, align 8
  %100 = bitcast double* %force_round_fr_mem345 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %100)
  store volatile double %expt_even_mid_block_hi__unroll_x344, double* %force_round_fr_mem345, align 8
  %force_roundfr_rounded346 = load volatile double* %force_round_fr_mem345, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %100)
  %force_round_fr_mem348 = alloca double, align 8
  %101 = bitcast double* %force_round_fr_mem348 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %101)
  store volatile double %force_roundfr_rounded346, double* %force_round_fr_mem348, align 8
  %force_roundfr_rounded349 = load volatile double* %force_round_fr_mem348, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %101)
  %expt_even_mid_block_res_hi_abtest350 = fcmp olt double %force_roundfr_rounded343, %force_roundfr_rounded349
  %expt_even_mid_block_res_hi_abmax351 = select i1 %expt_even_mid_block_res_hi_abtest350, double %force_roundfr_rounded349, double %force_roundfr_rounded343
  br label %expt_even_merge352

expt_even_merge352:                               ; preds = %expt_even_contains_zero337, %expt_even_both_are_neg324, %expt_even_both_are_pos309
  %expt_lo353 = phi double [ %force_roundfr_rounded315, %expt_even_both_are_pos309 ], [ %force_roundfr_rounded330, %expt_even_both_are_neg324 ], [ 0.000000e+00, %expt_even_contains_zero337 ]
  %expt_hi354 = phi double [ %force_roundfr_rounded321, %expt_even_both_are_pos309 ], [ %force_roundfr_rounded336, %expt_even_both_are_neg324 ], [ %expt_even_mid_block_res_hi_abmax351, %expt_even_contains_zero337 ]
  %neg_lo355 = fsub double -0.000000e+00, %expt_hi354
  %neg_hi356 = fsub double -0.000000e+00, %expt_lo353
  %mul_hi_a357 = fmul double %neg_lo355, %e_lo
  %force_round_fr_mem358 = alloca double, align 8
  %102 = bitcast double* %force_round_fr_mem358 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %102)
  store volatile double %mul_hi_a357, double* %force_round_fr_mem358, align 8
  %force_roundfr_rounded359 = load volatile double* %force_round_fr_mem358, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %102)
  %mul_hi_b360 = fmul double %neg_lo355, %e_hi
  %force_round_fr_mem361 = alloca double, align 8
  %103 = bitcast double* %force_round_fr_mem361 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %103)
  store volatile double %mul_hi_b360, double* %force_round_fr_mem361, align 8
  %force_roundfr_rounded362 = load volatile double* %force_round_fr_mem361, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %103)
  %mul_hi_c363 = fmul double %neg_hi356, %e_lo
  %force_round_fr_mem364 = alloca double, align 8
  %104 = bitcast double* %force_round_fr_mem364 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %104)
  store volatile double %mul_hi_c363, double* %force_round_fr_mem364, align 8
  %force_roundfr_rounded365 = load volatile double* %force_round_fr_mem364, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %104)
  %mul_hi_d366 = fmul double %neg_hi356, %e_hi
  %force_round_fr_mem367 = alloca double, align 8
  %105 = bitcast double* %force_round_fr_mem367 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %105)
  store volatile double %mul_hi_d366, double* %force_round_fr_mem367, align 8
  %force_roundfr_rounded368 = load volatile double* %force_round_fr_mem367, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %105)
  %mul_hi_ab_test369 = fcmp olt double %force_roundfr_rounded359, %force_roundfr_rounded362
  %mul_hi_cd_test370 = fcmp olt double %force_roundfr_rounded365, %force_roundfr_rounded368
  %mul_hi_ab_max371 = select i1 %mul_hi_ab_test369, double %force_roundfr_rounded362, double %force_roundfr_rounded359
  %mul_hi_cd_max372 = select i1 %mul_hi_cd_test370, double %force_roundfr_rounded368, double %force_roundfr_rounded365
  %mul_hi_abcd_test373 = fcmp olt double %mul_hi_ab_max371, %mul_hi_cd_max372
  %mul_hi_abcd_max374 = select i1 %mul_hi_abcd_test373, double %mul_hi_cd_max372, double %mul_hi_ab_max371
  call void @set_rounding_mode_down()
  %force_round_fr_mem376 = alloca double, align 8
  %106 = bitcast double* %force_round_fr_mem376 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %106)
  store volatile double %mul_hi_a357, double* %force_round_fr_mem376, align 8
  %force_roundfr_rounded377 = load volatile double* %force_round_fr_mem376, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %106)
  %force_round_fr_mem379 = alloca double, align 8
  %107 = bitcast double* %force_round_fr_mem379 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %107)
  store volatile double %mul_hi_b360, double* %force_round_fr_mem379, align 8
  %force_roundfr_rounded380 = load volatile double* %force_round_fr_mem379, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %107)
  %force_round_fr_mem382 = alloca double, align 8
  %108 = bitcast double* %force_round_fr_mem382 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %108)
  store volatile double %mul_hi_c363, double* %force_round_fr_mem382, align 8
  %force_roundfr_rounded383 = load volatile double* %force_round_fr_mem382, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %108)
  %force_round_fr_mem385 = alloca double, align 8
  %109 = bitcast double* %force_round_fr_mem385 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %109)
  store volatile double %mul_hi_d366, double* %force_round_fr_mem385, align 8
  %force_roundfr_rounded386 = load volatile double* %force_round_fr_mem385, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %109)
  %mul_lo_ab_test387 = fcmp olt double %force_roundfr_rounded380, %force_roundfr_rounded377
  %mul_lo_cd_test388 = fcmp olt double %force_roundfr_rounded386, %force_roundfr_rounded383
  %mul_lo_cd_min389 = select i1 %mul_lo_cd_test388, double %force_roundfr_rounded386, double %force_roundfr_rounded383
  %mul_lo_ab_min390 = select i1 %mul_lo_ab_test387, double %force_roundfr_rounded380, double %force_roundfr_rounded377
  %mul_lo_abcd_test391 = fcmp olt double %mul_lo_cd_min389, %mul_lo_ab_min390
  %mul_lo_abcd_min392 = select i1 %mul_lo_abcd_test391, double %mul_lo_cd_min389, double %mul_lo_ab_min390
  %add_lo395 = fsub double 4.900000e+01, %mul_hi_abcd_max374
  %force_round_fr_mem396 = alloca double, align 8
  %110 = bitcast double* %force_round_fr_mem396 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %110)
  store volatile double %add_lo395, double* %force_round_fr_mem396, align 8
  %force_roundfr_rounded397 = load volatile double* %force_round_fr_mem396, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %110)
  call void @set_rounding_mode_up()
  %add_hi398 = fsub double 4.900000e+01, %mul_lo_abcd_min392
  %force_round_fr_mem399 = alloca double, align 8
  %111 = bitcast double* %force_round_fr_mem399 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %111)
  store volatile double %add_hi398, double* %force_round_fr_mem399, align 8
  %force_roundfr_rounded400 = load volatile double* %force_round_fr_mem399, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %111)
  call void @set_rounding_mode_down()
  %sub_lo = fsub double 1.400000e+01, %force_roundfr_rounded400
  %force_round_fr_mem401 = alloca double, align 8
  %112 = bitcast double* %force_round_fr_mem401 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %112)
  store volatile double %sub_lo, double* %force_round_fr_mem401, align 8
  %force_roundfr_rounded402 = load volatile double* %force_round_fr_mem401, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %112)
  call void @set_rounding_mode_up()
  %sub_hi = fsub double 1.400000e+01, %force_roundfr_rounded397
  %force_round_fr_mem403 = alloca double, align 8
  %113 = bitcast double* %force_round_fr_mem403 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %113)
  store volatile double %sub_hi, double* %force_round_fr_mem403, align 8
  %force_roundfr_rounded404 = load volatile double* %force_round_fr_mem403, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %113)
  %neg_lo405 = fsub double -0.000000e+00, %force_roundfr_rounded404
  %neg_hi406 = fsub double -0.000000e+00, %force_roundfr_rounded402
  %div_hi_a407 = fdiv double %neg_lo405, %f_lo
  %force_round_fr_mem408 = alloca double, align 8
  %114 = bitcast double* %force_round_fr_mem408 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %114)
  store volatile double %div_hi_a407, double* %force_round_fr_mem408, align 8
  %force_roundfr_rounded409 = load volatile double* %force_round_fr_mem408, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %114)
  %div_hi_b410 = fdiv double %neg_lo405, %f_hi
  %force_round_fr_mem411 = alloca double, align 8
  %115 = bitcast double* %force_round_fr_mem411 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %115)
  store volatile double %div_hi_b410, double* %force_round_fr_mem411, align 8
  %force_roundfr_rounded412 = load volatile double* %force_round_fr_mem411, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %115)
  %div_hi_c413 = fdiv double %neg_hi406, %f_lo
  %force_round_fr_mem414 = alloca double, align 8
  %116 = bitcast double* %force_round_fr_mem414 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %116)
  store volatile double %div_hi_c413, double* %force_round_fr_mem414, align 8
  %force_roundfr_rounded415 = load volatile double* %force_round_fr_mem414, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %116)
  %div_hi_d416 = fdiv double %neg_hi406, %f_hi
  %force_round_fr_mem417 = alloca double, align 8
  %117 = bitcast double* %force_round_fr_mem417 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %117)
  store volatile double %div_hi_d416, double* %force_round_fr_mem417, align 8
  %force_roundfr_rounded418 = load volatile double* %force_round_fr_mem417, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %117)
  %div_hi_ab_test419 = fcmp olt double %force_roundfr_rounded409, %force_roundfr_rounded412
  %div_hi_cd_test420 = fcmp olt double %force_roundfr_rounded415, %force_roundfr_rounded418
  %div_hi_ab_max421 = select i1 %div_hi_ab_test419, double %force_roundfr_rounded412, double %force_roundfr_rounded409
  %div_hi_cd_max422 = select i1 %div_hi_cd_test420, double %force_roundfr_rounded418, double %force_roundfr_rounded415
  %div_hi_abcd_test423 = fcmp olt double %div_hi_ab_max421, %div_hi_cd_max422
  %div_hi_abcd_max424 = select i1 %div_hi_abcd_test423, double %div_hi_cd_max422, double %div_hi_ab_max421
  call void @set_rounding_mode_down()
  %force_round_fr_mem426 = alloca double, align 8
  %118 = bitcast double* %force_round_fr_mem426 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %118)
  store volatile double %div_hi_a407, double* %force_round_fr_mem426, align 8
  %force_roundfr_rounded427 = load volatile double* %force_round_fr_mem426, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %118)
  %force_round_fr_mem429 = alloca double, align 8
  %119 = bitcast double* %force_round_fr_mem429 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %119)
  store volatile double %div_hi_b410, double* %force_round_fr_mem429, align 8
  %force_roundfr_rounded430 = load volatile double* %force_round_fr_mem429, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %119)
  %force_round_fr_mem432 = alloca double, align 8
  %120 = bitcast double* %force_round_fr_mem432 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %120)
  store volatile double %div_hi_c413, double* %force_round_fr_mem432, align 8
  %force_roundfr_rounded433 = load volatile double* %force_round_fr_mem432, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %120)
  %force_round_fr_mem435 = alloca double, align 8
  %121 = bitcast double* %force_round_fr_mem435 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %121)
  store volatile double %div_hi_d416, double* %force_round_fr_mem435, align 8
  %force_roundfr_rounded436 = load volatile double* %force_round_fr_mem435, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %121)
  %div_lo_ab_test437 = fcmp olt double %force_roundfr_rounded430, %force_roundfr_rounded427
  %div_lo_cd_test438 = fcmp olt double %force_roundfr_rounded436, %force_roundfr_rounded433
  %div_lo_cd_min439 = select i1 %div_lo_cd_test438, double %force_roundfr_rounded436, double %force_roundfr_rounded433
  %div_lo_ab_min440 = select i1 %div_lo_ab_test437, double %force_roundfr_rounded430, double %force_roundfr_rounded427
  %div_lo_abcd_test441 = fcmp olt double %div_lo_cd_min439, %div_lo_ab_min440
  %div_lo_abcd_min442 = select i1 %div_lo_abcd_test441, double %div_lo_cd_min439, double %div_lo_ab_min440
  call void @set_rounding_mode_up()
  %mul_hi_a443 = fmul double %div_lo_abcd_min442, 3.500000e+01
  %force_round_fr_mem444 = alloca double, align 8
  %122 = bitcast double* %force_round_fr_mem444 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %122)
  store volatile double %mul_hi_a443, double* %force_round_fr_mem444, align 8
  %force_roundfr_rounded445 = load volatile double* %force_round_fr_mem444, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %122)
  %mul_hi_b446 = fmul double %div_hi_abcd_max424, 3.500000e+01
  %force_round_fr_mem447 = alloca double, align 8
  %123 = bitcast double* %force_round_fr_mem447 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %123)
  store volatile double %mul_hi_b446, double* %force_round_fr_mem447, align 8
  %force_roundfr_rounded448 = load volatile double* %force_round_fr_mem447, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %123)
  %force_round_fr_mem450 = alloca double, align 8
  %124 = bitcast double* %force_round_fr_mem450 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %124)
  store volatile double %mul_hi_a443, double* %force_round_fr_mem450, align 8
  %force_roundfr_rounded451 = load volatile double* %force_round_fr_mem450, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %124)
  %force_round_fr_mem453 = alloca double, align 8
  %125 = bitcast double* %force_round_fr_mem453 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %125)
  store volatile double %mul_hi_b446, double* %force_round_fr_mem453, align 8
  %force_roundfr_rounded454 = load volatile double* %force_round_fr_mem453, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %125)
  %mul_hi_ab_test455 = fcmp olt double %force_roundfr_rounded445, %force_roundfr_rounded448
  %mul_hi_cd_test456 = fcmp olt double %force_roundfr_rounded451, %force_roundfr_rounded454
  %mul_hi_ab_max457 = select i1 %mul_hi_ab_test455, double %force_roundfr_rounded448, double %force_roundfr_rounded445
  %mul_hi_cd_max458 = select i1 %mul_hi_cd_test456, double %force_roundfr_rounded454, double %force_roundfr_rounded451
  %mul_hi_abcd_test459 = fcmp olt double %mul_hi_ab_max457, %mul_hi_cd_max458
  %mul_hi_abcd_max460 = select i1 %mul_hi_abcd_test459, double %mul_hi_cd_max458, double %mul_hi_ab_max457
  call void @set_rounding_mode_down()
  %force_round_fr_mem462 = alloca double, align 8
  %126 = bitcast double* %force_round_fr_mem462 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %126)
  store volatile double %mul_hi_a443, double* %force_round_fr_mem462, align 8
  %force_roundfr_rounded463 = load volatile double* %force_round_fr_mem462, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %126)
  %force_round_fr_mem465 = alloca double, align 8
  %127 = bitcast double* %force_round_fr_mem465 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %127)
  store volatile double %mul_hi_b446, double* %force_round_fr_mem465, align 8
  %force_roundfr_rounded466 = load volatile double* %force_round_fr_mem465, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %127)
  %force_round_fr_mem468 = alloca double, align 8
  %128 = bitcast double* %force_round_fr_mem468 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %128)
  store volatile double %mul_hi_a443, double* %force_round_fr_mem468, align 8
  %force_roundfr_rounded469 = load volatile double* %force_round_fr_mem468, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %128)
  %force_round_fr_mem471 = alloca double, align 8
  %129 = bitcast double* %force_round_fr_mem471 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %129)
  store volatile double %mul_hi_b446, double* %force_round_fr_mem471, align 8
  %force_roundfr_rounded472 = load volatile double* %force_round_fr_mem471, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %129)
  %mul_lo_ab_test473 = fcmp olt double %force_roundfr_rounded466, %force_roundfr_rounded463
  %mul_lo_cd_test474 = fcmp olt double %force_roundfr_rounded472, %force_roundfr_rounded469
  %mul_lo_cd_min475 = select i1 %mul_lo_cd_test474, double %force_roundfr_rounded472, double %force_roundfr_rounded469
  %mul_lo_ab_min476 = select i1 %mul_lo_ab_test473, double %force_roundfr_rounded466, double %force_roundfr_rounded463
  %mul_lo_abcd_test477 = fcmp olt double %mul_lo_cd_min475, %mul_lo_ab_min476
  %mul_lo_abcd_min478 = select i1 %mul_lo_abcd_test477, double %mul_lo_cd_min475, double %mul_lo_ab_min476
  %add_lo479 = fadd double %mul_lo_abcd_min478, %g_lo
  %force_round_fr_mem480 = alloca double, align 8
  %130 = bitcast double* %force_round_fr_mem480 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %130)
  store volatile double %add_lo479, double* %force_round_fr_mem480, align 8
  %force_roundfr_rounded481 = load volatile double* %force_round_fr_mem480, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %130)
  call void @set_rounding_mode_up()
  %add_hi482 = fadd double %mul_hi_abcd_max460, %g_hi
  %force_round_fr_mem483 = alloca double, align 8
  %131 = bitcast double* %force_round_fr_mem483 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %131)
  store volatile double %add_hi482, double* %force_round_fr_mem483, align 8
  %force_roundfr_rounded484 = load volatile double* %force_round_fr_mem483, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %131)
  %expt_even_are_both_pos_p485 = fcmp ogt double %force_roundfr_rounded481, 0.000000e+00
  br i1 %expt_even_are_both_pos_p485, label %expt_even_both_are_pos486, label %expt_even_both_are_not_pos505

expt_even_both_are_pos486:                        ; preds = %expt_even_merge352
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x487 = fmul double %force_roundfr_rounded481, %force_roundfr_rounded481
  %force_round_fr_mem488 = alloca double, align 8
  %132 = bitcast double* %force_round_fr_mem488 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %132)
  store volatile double %expt_even_pos_block_res_lo_unroll_x487, double* %force_round_fr_mem488, align 8
  %force_roundfr_rounded489 = load volatile double* %force_round_fr_mem488, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %132)
  %expt_even_pos_block_res_lo_unroll_x490 = fmul double %force_roundfr_rounded489, %force_roundfr_rounded489
  %force_round_fr_mem491 = alloca double, align 8
  %133 = bitcast double* %force_round_fr_mem491 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %133)
  store volatile double %expt_even_pos_block_res_lo_unroll_x490, double* %force_round_fr_mem491, align 8
  %force_roundfr_rounded492 = load volatile double* %force_round_fr_mem491, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %133)
  %force_round_fr_mem494 = alloca double, align 8
  %134 = bitcast double* %force_round_fr_mem494 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %134)
  store volatile double %force_roundfr_rounded492, double* %force_round_fr_mem494, align 8
  %force_roundfr_rounded495 = load volatile double* %force_round_fr_mem494, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %134)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x496 = fmul double %force_roundfr_rounded484, %force_roundfr_rounded484
  %force_round_fr_mem497 = alloca double, align 8
  %135 = bitcast double* %force_round_fr_mem497 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %135)
  store volatile double %expt_even_pos_block_res_hi_unroll_x496, double* %force_round_fr_mem497, align 8
  %force_roundfr_rounded498 = load volatile double* %force_round_fr_mem497, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %135)
  %expt_even_pos_block_res_hi_unroll_x499 = fmul double %force_roundfr_rounded498, %force_roundfr_rounded498
  %force_round_fr_mem500 = alloca double, align 8
  %136 = bitcast double* %force_round_fr_mem500 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %136)
  store volatile double %expt_even_pos_block_res_hi_unroll_x499, double* %force_round_fr_mem500, align 8
  %force_roundfr_rounded501 = load volatile double* %force_round_fr_mem500, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %136)
  %force_round_fr_mem503 = alloca double, align 8
  %137 = bitcast double* %force_round_fr_mem503 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %137)
  store volatile double %force_roundfr_rounded501, double* %force_round_fr_mem503, align 8
  %force_roundfr_rounded504 = load volatile double* %force_round_fr_mem503, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %137)
  br label %expt_even_merge547

expt_even_both_are_not_pos505:                    ; preds = %expt_even_merge352
  %expt_even_are_both_neg_p506 = fcmp olt double %force_roundfr_rounded484, 0.000000e+00
  br i1 %expt_even_are_both_neg_p506, label %expt_even_both_are_neg507, label %expt_even_contains_zero526

expt_even_both_are_neg507:                        ; preds = %expt_even_both_are_not_pos505
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x508 = fmul double %force_roundfr_rounded484, %force_roundfr_rounded484
  %force_round_fr_mem509 = alloca double, align 8
  %138 = bitcast double* %force_round_fr_mem509 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %138)
  store volatile double %expt_even_neg_block_res_lo_unroll_x508, double* %force_round_fr_mem509, align 8
  %force_roundfr_rounded510 = load volatile double* %force_round_fr_mem509, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %138)
  %expt_even_neg_block_res_lo_unroll_x511 = fmul double %force_roundfr_rounded510, %force_roundfr_rounded510
  %force_round_fr_mem512 = alloca double, align 8
  %139 = bitcast double* %force_round_fr_mem512 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %139)
  store volatile double %expt_even_neg_block_res_lo_unroll_x511, double* %force_round_fr_mem512, align 8
  %force_roundfr_rounded513 = load volatile double* %force_round_fr_mem512, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %139)
  %force_round_fr_mem515 = alloca double, align 8
  %140 = bitcast double* %force_round_fr_mem515 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %140)
  store volatile double %force_roundfr_rounded513, double* %force_round_fr_mem515, align 8
  %force_roundfr_rounded516 = load volatile double* %force_round_fr_mem515, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %140)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x517 = fmul double %force_roundfr_rounded481, %force_roundfr_rounded481
  %force_round_fr_mem518 = alloca double, align 8
  %141 = bitcast double* %force_round_fr_mem518 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %141)
  store volatile double %expt_even_pos_block_res_hi_unroll_x517, double* %force_round_fr_mem518, align 8
  %force_roundfr_rounded519 = load volatile double* %force_round_fr_mem518, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %141)
  %expt_even_pos_block_res_hi_unroll_x520 = fmul double %force_roundfr_rounded519, %force_roundfr_rounded519
  %force_round_fr_mem521 = alloca double, align 8
  %142 = bitcast double* %force_round_fr_mem521 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %142)
  store volatile double %expt_even_pos_block_res_hi_unroll_x520, double* %force_round_fr_mem521, align 8
  %force_roundfr_rounded522 = load volatile double* %force_round_fr_mem521, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %142)
  %force_round_fr_mem524 = alloca double, align 8
  %143 = bitcast double* %force_round_fr_mem524 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %143)
  store volatile double %force_roundfr_rounded522, double* %force_round_fr_mem524, align 8
  %force_roundfr_rounded525 = load volatile double* %force_round_fr_mem524, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %143)
  br label %expt_even_merge547

expt_even_contains_zero526:                       ; preds = %expt_even_both_are_not_pos505
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x527 = fmul double %force_roundfr_rounded481, %force_roundfr_rounded481
  %force_round_fr_mem528 = alloca double, align 8
  %144 = bitcast double* %force_round_fr_mem528 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %144)
  store volatile double %expt_even_mid_block_lo__unroll_x527, double* %force_round_fr_mem528, align 8
  %force_roundfr_rounded529 = load volatile double* %force_round_fr_mem528, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %144)
  %expt_even_mid_block_lo__unroll_x530 = fmul double %force_roundfr_rounded529, %force_roundfr_rounded529
  %force_round_fr_mem531 = alloca double, align 8
  %145 = bitcast double* %force_round_fr_mem531 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %145)
  store volatile double %expt_even_mid_block_lo__unroll_x530, double* %force_round_fr_mem531, align 8
  %force_roundfr_rounded532 = load volatile double* %force_round_fr_mem531, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %145)
  %force_round_fr_mem534 = alloca double, align 8
  %146 = bitcast double* %force_round_fr_mem534 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %146)
  store volatile double %force_roundfr_rounded532, double* %force_round_fr_mem534, align 8
  %force_roundfr_rounded535 = load volatile double* %force_round_fr_mem534, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %146)
  %expt_even_mid_block_hi__unroll_x536 = fmul double %force_roundfr_rounded484, %force_roundfr_rounded484
  %force_round_fr_mem537 = alloca double, align 8
  %147 = bitcast double* %force_round_fr_mem537 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %147)
  store volatile double %expt_even_mid_block_hi__unroll_x536, double* %force_round_fr_mem537, align 8
  %force_roundfr_rounded538 = load volatile double* %force_round_fr_mem537, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %147)
  %expt_even_mid_block_hi__unroll_x539 = fmul double %force_roundfr_rounded538, %force_roundfr_rounded538
  %force_round_fr_mem540 = alloca double, align 8
  %148 = bitcast double* %force_round_fr_mem540 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %148)
  store volatile double %expt_even_mid_block_hi__unroll_x539, double* %force_round_fr_mem540, align 8
  %force_roundfr_rounded541 = load volatile double* %force_round_fr_mem540, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %148)
  %force_round_fr_mem543 = alloca double, align 8
  %149 = bitcast double* %force_round_fr_mem543 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %149)
  store volatile double %force_roundfr_rounded541, double* %force_round_fr_mem543, align 8
  %force_roundfr_rounded544 = load volatile double* %force_round_fr_mem543, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %149)
  %expt_even_mid_block_res_hi_abtest545 = fcmp olt double %force_roundfr_rounded535, %force_roundfr_rounded544
  %expt_even_mid_block_res_hi_abmax546 = select i1 %expt_even_mid_block_res_hi_abtest545, double %force_roundfr_rounded544, double %force_roundfr_rounded535
  br label %expt_even_merge547

expt_even_merge547:                               ; preds = %expt_even_contains_zero526, %expt_even_both_are_neg507, %expt_even_both_are_pos486
  %expt_lo548 = phi double [ %force_roundfr_rounded495, %expt_even_both_are_pos486 ], [ %force_roundfr_rounded516, %expt_even_both_are_neg507 ], [ 0.000000e+00, %expt_even_contains_zero526 ]
  %expt_hi549 = phi double [ %force_roundfr_rounded504, %expt_even_both_are_pos486 ], [ %force_roundfr_rounded525, %expt_even_both_are_neg507 ], [ %expt_even_mid_block_res_hi_abmax546, %expt_even_contains_zero526 ]
  %expt_even_are_both_pos_p552 = fcmp olt double %expt_hi549, -0.000000e+00
  br i1 %expt_even_are_both_pos_p552, label %expt_even_both_are_pos553, label %expt_even_both_are_not_pos572

expt_even_both_are_pos553:                        ; preds = %expt_even_merge547
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x554 = fmul double %expt_hi549, %expt_hi549
  %force_round_fr_mem555 = alloca double, align 8
  %150 = bitcast double* %force_round_fr_mem555 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %150)
  store volatile double %expt_even_pos_block_res_lo_unroll_x554, double* %force_round_fr_mem555, align 8
  %force_roundfr_rounded556 = load volatile double* %force_round_fr_mem555, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %150)
  %expt_even_pos_block_res_lo_unroll_x557 = fmul double %force_roundfr_rounded556, %force_roundfr_rounded556
  %force_round_fr_mem558 = alloca double, align 8
  %151 = bitcast double* %force_round_fr_mem558 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %151)
  store volatile double %expt_even_pos_block_res_lo_unroll_x557, double* %force_round_fr_mem558, align 8
  %force_roundfr_rounded559 = load volatile double* %force_round_fr_mem558, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %151)
  %force_round_fr_mem561 = alloca double, align 8
  %152 = bitcast double* %force_round_fr_mem561 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %152)
  store volatile double %force_roundfr_rounded559, double* %force_round_fr_mem561, align 8
  %force_roundfr_rounded562 = load volatile double* %force_round_fr_mem561, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %152)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x563 = fmul double %expt_lo548, %expt_lo548
  %force_round_fr_mem564 = alloca double, align 8
  %153 = bitcast double* %force_round_fr_mem564 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %153)
  store volatile double %expt_even_pos_block_res_hi_unroll_x563, double* %force_round_fr_mem564, align 8
  %force_roundfr_rounded565 = load volatile double* %force_round_fr_mem564, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %153)
  %expt_even_pos_block_res_hi_unroll_x566 = fmul double %force_roundfr_rounded565, %force_roundfr_rounded565
  %force_round_fr_mem567 = alloca double, align 8
  %154 = bitcast double* %force_round_fr_mem567 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %154)
  store volatile double %expt_even_pos_block_res_hi_unroll_x566, double* %force_round_fr_mem567, align 8
  %force_roundfr_rounded568 = load volatile double* %force_round_fr_mem567, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %154)
  %force_round_fr_mem570 = alloca double, align 8
  %155 = bitcast double* %force_round_fr_mem570 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %155)
  store volatile double %force_roundfr_rounded568, double* %force_round_fr_mem570, align 8
  %force_roundfr_rounded571 = load volatile double* %force_round_fr_mem570, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %155)
  br label %expt_even_merge614

expt_even_both_are_not_pos572:                    ; preds = %expt_even_merge547
  %expt_even_are_both_neg_p573 = fcmp ogt double %expt_lo548, -0.000000e+00
  br i1 %expt_even_are_both_neg_p573, label %expt_even_both_are_neg574, label %expt_even_contains_zero593

expt_even_both_are_neg574:                        ; preds = %expt_even_both_are_not_pos572
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x575 = fmul double %expt_lo548, %expt_lo548
  %force_round_fr_mem576 = alloca double, align 8
  %156 = bitcast double* %force_round_fr_mem576 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %156)
  store volatile double %expt_even_neg_block_res_lo_unroll_x575, double* %force_round_fr_mem576, align 8
  %force_roundfr_rounded577 = load volatile double* %force_round_fr_mem576, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %156)
  %expt_even_neg_block_res_lo_unroll_x578 = fmul double %force_roundfr_rounded577, %force_roundfr_rounded577
  %force_round_fr_mem579 = alloca double, align 8
  %157 = bitcast double* %force_round_fr_mem579 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %157)
  store volatile double %expt_even_neg_block_res_lo_unroll_x578, double* %force_round_fr_mem579, align 8
  %force_roundfr_rounded580 = load volatile double* %force_round_fr_mem579, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %157)
  %force_round_fr_mem582 = alloca double, align 8
  %158 = bitcast double* %force_round_fr_mem582 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %158)
  store volatile double %force_roundfr_rounded580, double* %force_round_fr_mem582, align 8
  %force_roundfr_rounded583 = load volatile double* %force_round_fr_mem582, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %158)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x584 = fmul double %expt_hi549, %expt_hi549
  %force_round_fr_mem585 = alloca double, align 8
  %159 = bitcast double* %force_round_fr_mem585 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %159)
  store volatile double %expt_even_pos_block_res_hi_unroll_x584, double* %force_round_fr_mem585, align 8
  %force_roundfr_rounded586 = load volatile double* %force_round_fr_mem585, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %159)
  %expt_even_pos_block_res_hi_unroll_x587 = fmul double %force_roundfr_rounded586, %force_roundfr_rounded586
  %force_round_fr_mem588 = alloca double, align 8
  %160 = bitcast double* %force_round_fr_mem588 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %160)
  store volatile double %expt_even_pos_block_res_hi_unroll_x587, double* %force_round_fr_mem588, align 8
  %force_roundfr_rounded589 = load volatile double* %force_round_fr_mem588, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %160)
  %force_round_fr_mem591 = alloca double, align 8
  %161 = bitcast double* %force_round_fr_mem591 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %161)
  store volatile double %force_roundfr_rounded589, double* %force_round_fr_mem591, align 8
  %force_roundfr_rounded592 = load volatile double* %force_round_fr_mem591, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %161)
  br label %expt_even_merge614

expt_even_contains_zero593:                       ; preds = %expt_even_both_are_not_pos572
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x594 = fmul double %expt_hi549, %expt_hi549
  %force_round_fr_mem595 = alloca double, align 8
  %162 = bitcast double* %force_round_fr_mem595 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %162)
  store volatile double %expt_even_mid_block_lo__unroll_x594, double* %force_round_fr_mem595, align 8
  %force_roundfr_rounded596 = load volatile double* %force_round_fr_mem595, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %162)
  %expt_even_mid_block_lo__unroll_x597 = fmul double %force_roundfr_rounded596, %force_roundfr_rounded596
  %force_round_fr_mem598 = alloca double, align 8
  %163 = bitcast double* %force_round_fr_mem598 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %163)
  store volatile double %expt_even_mid_block_lo__unroll_x597, double* %force_round_fr_mem598, align 8
  %force_roundfr_rounded599 = load volatile double* %force_round_fr_mem598, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %163)
  %force_round_fr_mem601 = alloca double, align 8
  %164 = bitcast double* %force_round_fr_mem601 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %164)
  store volatile double %force_roundfr_rounded599, double* %force_round_fr_mem601, align 8
  %force_roundfr_rounded602 = load volatile double* %force_round_fr_mem601, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %164)
  %expt_even_mid_block_hi__unroll_x603 = fmul double %expt_lo548, %expt_lo548
  %force_round_fr_mem604 = alloca double, align 8
  %165 = bitcast double* %force_round_fr_mem604 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %165)
  store volatile double %expt_even_mid_block_hi__unroll_x603, double* %force_round_fr_mem604, align 8
  %force_roundfr_rounded605 = load volatile double* %force_round_fr_mem604, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %165)
  %expt_even_mid_block_hi__unroll_x606 = fmul double %force_roundfr_rounded605, %force_roundfr_rounded605
  %force_round_fr_mem607 = alloca double, align 8
  %166 = bitcast double* %force_round_fr_mem607 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %166)
  store volatile double %expt_even_mid_block_hi__unroll_x606, double* %force_round_fr_mem607, align 8
  %force_roundfr_rounded608 = load volatile double* %force_round_fr_mem607, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %166)
  %force_round_fr_mem610 = alloca double, align 8
  %167 = bitcast double* %force_round_fr_mem610 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %167)
  store volatile double %force_roundfr_rounded608, double* %force_round_fr_mem610, align 8
  %force_roundfr_rounded611 = load volatile double* %force_round_fr_mem610, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %167)
  %expt_even_mid_block_res_hi_abtest612 = fcmp olt double %force_roundfr_rounded602, %force_roundfr_rounded611
  %expt_even_mid_block_res_hi_abmax613 = select i1 %expt_even_mid_block_res_hi_abtest612, double %force_roundfr_rounded611, double %force_roundfr_rounded602
  br label %expt_even_merge614

expt_even_merge614:                               ; preds = %expt_even_contains_zero593, %expt_even_both_are_neg574, %expt_even_both_are_pos553
  %expt_lo615 = phi double [ %force_roundfr_rounded562, %expt_even_both_are_pos553 ], [ %force_roundfr_rounded583, %expt_even_both_are_neg574 ], [ 0.000000e+00, %expt_even_contains_zero593 ]
  %expt_hi616 = phi double [ %force_roundfr_rounded571, %expt_even_both_are_pos553 ], [ %force_roundfr_rounded592, %expt_even_both_are_neg574 ], [ %expt_even_mid_block_res_hi_abmax613, %expt_even_contains_zero593 ]
  call void @set_rounding_mode_down()
  %add_lo619 = fadd double %expt_lo615, -8.500000e+01
  %force_round_fr_mem620 = alloca double, align 8
  %168 = bitcast double* %force_round_fr_mem620 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %168)
  store volatile double %add_lo619, double* %force_round_fr_mem620, align 8
  %force_roundfr_rounded621 = load volatile double* %force_round_fr_mem620, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %168)
  call void @set_rounding_mode_up()
  %add_hi622 = fadd double %expt_hi616, -8.500000e+01
  %force_round_fr_mem623 = alloca double, align 8
  %169 = bitcast double* %force_round_fr_mem623 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %169)
  store volatile double %add_hi622, double* %force_round_fr_mem623, align 8
  %force_roundfr_rounded624 = load volatile double* %force_round_fr_mem623, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %169)
  %div_hi_a625 = fdiv double %h_lo, %force_roundfr_rounded621
  %force_round_fr_mem626 = alloca double, align 8
  %170 = bitcast double* %force_round_fr_mem626 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %170)
  store volatile double %div_hi_a625, double* %force_round_fr_mem626, align 8
  %force_roundfr_rounded627 = load volatile double* %force_round_fr_mem626, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %170)
  %div_hi_b628 = fdiv double %h_lo, %force_roundfr_rounded624
  %force_round_fr_mem629 = alloca double, align 8
  %171 = bitcast double* %force_round_fr_mem629 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %171)
  store volatile double %div_hi_b628, double* %force_round_fr_mem629, align 8
  %force_roundfr_rounded630 = load volatile double* %force_round_fr_mem629, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %171)
  %div_hi_c631 = fdiv double %h_hi, %force_roundfr_rounded621
  %force_round_fr_mem632 = alloca double, align 8
  %172 = bitcast double* %force_round_fr_mem632 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %172)
  store volatile double %div_hi_c631, double* %force_round_fr_mem632, align 8
  %force_roundfr_rounded633 = load volatile double* %force_round_fr_mem632, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %172)
  %div_hi_d634 = fdiv double %h_hi, %force_roundfr_rounded624
  %force_round_fr_mem635 = alloca double, align 8
  %173 = bitcast double* %force_round_fr_mem635 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %173)
  store volatile double %div_hi_d634, double* %force_round_fr_mem635, align 8
  %force_roundfr_rounded636 = load volatile double* %force_round_fr_mem635, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %173)
  %div_hi_ab_test637 = fcmp olt double %force_roundfr_rounded627, %force_roundfr_rounded630
  %div_hi_cd_test638 = fcmp olt double %force_roundfr_rounded633, %force_roundfr_rounded636
  %div_hi_ab_max639 = select i1 %div_hi_ab_test637, double %force_roundfr_rounded630, double %force_roundfr_rounded627
  %div_hi_cd_max640 = select i1 %div_hi_cd_test638, double %force_roundfr_rounded636, double %force_roundfr_rounded633
  %div_hi_abcd_test641 = fcmp olt double %div_hi_ab_max639, %div_hi_cd_max640
  %div_hi_abcd_max642 = select i1 %div_hi_abcd_test641, double %div_hi_cd_max640, double %div_hi_ab_max639
  call void @set_rounding_mode_down()
  %force_round_fr_mem644 = alloca double, align 8
  %174 = bitcast double* %force_round_fr_mem644 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %174)
  store volatile double %div_hi_a625, double* %force_round_fr_mem644, align 8
  %force_roundfr_rounded645 = load volatile double* %force_round_fr_mem644, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %174)
  %force_round_fr_mem647 = alloca double, align 8
  %175 = bitcast double* %force_round_fr_mem647 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %175)
  store volatile double %div_hi_b628, double* %force_round_fr_mem647, align 8
  %force_roundfr_rounded648 = load volatile double* %force_round_fr_mem647, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %175)
  %force_round_fr_mem650 = alloca double, align 8
  %176 = bitcast double* %force_round_fr_mem650 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %176)
  store volatile double %div_hi_c631, double* %force_round_fr_mem650, align 8
  %force_roundfr_rounded651 = load volatile double* %force_round_fr_mem650, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %176)
  %force_round_fr_mem653 = alloca double, align 8
  %177 = bitcast double* %force_round_fr_mem653 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %177)
  store volatile double %div_hi_d634, double* %force_round_fr_mem653, align 8
  %force_roundfr_rounded654 = load volatile double* %force_round_fr_mem653, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %177)
  %div_lo_ab_test655 = fcmp olt double %force_roundfr_rounded648, %force_roundfr_rounded645
  %div_lo_cd_test656 = fcmp olt double %force_roundfr_rounded654, %force_roundfr_rounded651
  %div_lo_cd_min657 = select i1 %div_lo_cd_test656, double %force_roundfr_rounded654, double %force_roundfr_rounded651
  %div_lo_ab_min658 = select i1 %div_lo_ab_test655, double %force_roundfr_rounded648, double %force_roundfr_rounded645
  %div_lo_abcd_test659 = fcmp olt double %div_lo_cd_min657, %div_lo_ab_min658
  %div_lo_abcd_min660 = select i1 %div_lo_abcd_test659, double %div_lo_cd_min657, double %div_lo_ab_min658
  call void @set_rounding_mode_up()
  call void @set_rounding_mode_down()
  %add_lo665 = fadd double %div_lo_abcd_min660, %i_lo
  %force_round_fr_mem666 = alloca double, align 8
  %178 = bitcast double* %force_round_fr_mem666 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %178)
  store volatile double %add_lo665, double* %force_round_fr_mem666, align 8
  %force_roundfr_rounded667 = load volatile double* %force_round_fr_mem666, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %178)
  call void @set_rounding_mode_up()
  %add_hi668 = fadd double %div_hi_abcd_max642, %i_hi
  %force_round_fr_mem669 = alloca double, align 8
  %179 = bitcast double* %force_round_fr_mem669 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %179)
  store volatile double %add_hi668, double* %force_round_fr_mem669, align 8
  %force_roundfr_rounded670 = load volatile double* %force_round_fr_mem669, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %179)
  %mul_hi_a671 = fmul double %force_roundfr_rounded667, 3.200000e+01
  %force_round_fr_mem672 = alloca double, align 8
  %180 = bitcast double* %force_round_fr_mem672 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %180)
  store volatile double %mul_hi_a671, double* %force_round_fr_mem672, align 8
  %force_roundfr_rounded673 = load volatile double* %force_round_fr_mem672, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %180)
  %force_round_fr_mem675 = alloca double, align 8
  %181 = bitcast double* %force_round_fr_mem675 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %181)
  store volatile double %mul_hi_a671, double* %force_round_fr_mem675, align 8
  %force_roundfr_rounded676 = load volatile double* %force_round_fr_mem675, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %181)
  %mul_hi_c677 = fmul double %force_roundfr_rounded670, 3.200000e+01
  %force_round_fr_mem678 = alloca double, align 8
  %182 = bitcast double* %force_round_fr_mem678 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %182)
  store volatile double %mul_hi_c677, double* %force_round_fr_mem678, align 8
  %force_roundfr_rounded679 = load volatile double* %force_round_fr_mem678, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %182)
  %force_round_fr_mem681 = alloca double, align 8
  %183 = bitcast double* %force_round_fr_mem681 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %183)
  store volatile double %mul_hi_c677, double* %force_round_fr_mem681, align 8
  %force_roundfr_rounded682 = load volatile double* %force_round_fr_mem681, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %183)
  %mul_hi_ab_test683 = fcmp olt double %force_roundfr_rounded673, %force_roundfr_rounded676
  %mul_hi_cd_test684 = fcmp olt double %force_roundfr_rounded679, %force_roundfr_rounded682
  %mul_hi_ab_max685 = select i1 %mul_hi_ab_test683, double %force_roundfr_rounded676, double %force_roundfr_rounded673
  %mul_hi_cd_max686 = select i1 %mul_hi_cd_test684, double %force_roundfr_rounded682, double %force_roundfr_rounded679
  %mul_hi_abcd_test687 = fcmp olt double %mul_hi_ab_max685, %mul_hi_cd_max686
  %mul_hi_abcd_max688 = select i1 %mul_hi_abcd_test687, double %mul_hi_cd_max686, double %mul_hi_ab_max685
  call void @set_rounding_mode_down()
  %force_round_fr_mem690 = alloca double, align 8
  %184 = bitcast double* %force_round_fr_mem690 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %184)
  store volatile double %mul_hi_a671, double* %force_round_fr_mem690, align 8
  %force_roundfr_rounded691 = load volatile double* %force_round_fr_mem690, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %184)
  %force_round_fr_mem693 = alloca double, align 8
  %185 = bitcast double* %force_round_fr_mem693 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %185)
  store volatile double %mul_hi_a671, double* %force_round_fr_mem693, align 8
  %force_roundfr_rounded694 = load volatile double* %force_round_fr_mem693, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %185)
  %force_round_fr_mem696 = alloca double, align 8
  %186 = bitcast double* %force_round_fr_mem696 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %186)
  store volatile double %mul_hi_c677, double* %force_round_fr_mem696, align 8
  %force_roundfr_rounded697 = load volatile double* %force_round_fr_mem696, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %186)
  %force_round_fr_mem699 = alloca double, align 8
  %187 = bitcast double* %force_round_fr_mem699 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %187)
  store volatile double %mul_hi_c677, double* %force_round_fr_mem699, align 8
  %force_roundfr_rounded700 = load volatile double* %force_round_fr_mem699, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %187)
  %mul_lo_ab_test701 = fcmp olt double %force_roundfr_rounded694, %force_roundfr_rounded691
  %mul_lo_cd_test702 = fcmp olt double %force_roundfr_rounded700, %force_roundfr_rounded697
  %mul_lo_cd_min703 = select i1 %mul_lo_cd_test702, double %force_roundfr_rounded700, double %force_roundfr_rounded697
  %mul_lo_ab_min704 = select i1 %mul_lo_ab_test701, double %force_roundfr_rounded694, double %force_roundfr_rounded691
  %mul_lo_abcd_test705 = fcmp olt double %mul_lo_cd_min703, %mul_lo_ab_min704
  %mul_lo_abcd_min706 = select i1 %mul_lo_abcd_test705, double %mul_lo_cd_min703, double %mul_lo_ab_min704
  %expt_even_are_both_pos_p707 = fcmp ogt double %mul_lo_abcd_min706, 0.000000e+00
  call void @set_rounding_mode_up()
  br i1 %expt_even_are_both_pos_p707, label %expt_even_both_are_pos708, label %expt_even_both_are_not_pos727

expt_even_both_are_pos708:                        ; preds = %expt_even_merge614
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x709 = fmul double %mul_lo_abcd_min706, %mul_lo_abcd_min706
  %force_round_fr_mem710 = alloca double, align 8
  %188 = bitcast double* %force_round_fr_mem710 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %188)
  store volatile double %expt_even_pos_block_res_lo_unroll_x709, double* %force_round_fr_mem710, align 8
  %force_roundfr_rounded711 = load volatile double* %force_round_fr_mem710, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %188)
  %expt_even_pos_block_res_lo_unroll_x712 = fmul double %force_roundfr_rounded711, %force_roundfr_rounded711
  %force_round_fr_mem713 = alloca double, align 8
  %189 = bitcast double* %force_round_fr_mem713 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %189)
  store volatile double %expt_even_pos_block_res_lo_unroll_x712, double* %force_round_fr_mem713, align 8
  %force_roundfr_rounded714 = load volatile double* %force_round_fr_mem713, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %189)
  %force_round_fr_mem716 = alloca double, align 8
  %190 = bitcast double* %force_round_fr_mem716 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %190)
  store volatile double %force_roundfr_rounded714, double* %force_round_fr_mem716, align 8
  %force_roundfr_rounded717 = load volatile double* %force_round_fr_mem716, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %190)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x718 = fmul double %mul_hi_abcd_max688, %mul_hi_abcd_max688
  %force_round_fr_mem719 = alloca double, align 8
  %191 = bitcast double* %force_round_fr_mem719 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %191)
  store volatile double %expt_even_pos_block_res_hi_unroll_x718, double* %force_round_fr_mem719, align 8
  %force_roundfr_rounded720 = load volatile double* %force_round_fr_mem719, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %191)
  %expt_even_pos_block_res_hi_unroll_x721 = fmul double %force_roundfr_rounded720, %force_roundfr_rounded720
  %force_round_fr_mem722 = alloca double, align 8
  %192 = bitcast double* %force_round_fr_mem722 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %192)
  store volatile double %expt_even_pos_block_res_hi_unroll_x721, double* %force_round_fr_mem722, align 8
  %force_roundfr_rounded723 = load volatile double* %force_round_fr_mem722, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %192)
  %force_round_fr_mem725 = alloca double, align 8
  %193 = bitcast double* %force_round_fr_mem725 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %193)
  store volatile double %force_roundfr_rounded723, double* %force_round_fr_mem725, align 8
  %force_roundfr_rounded726 = load volatile double* %force_round_fr_mem725, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %193)
  br label %expt_even_merge769

expt_even_both_are_not_pos727:                    ; preds = %expt_even_merge614
  %expt_even_are_both_neg_p728 = fcmp olt double %mul_hi_abcd_max688, 0.000000e+00
  br i1 %expt_even_are_both_neg_p728, label %expt_even_both_are_neg729, label %expt_even_contains_zero748

expt_even_both_are_neg729:                        ; preds = %expt_even_both_are_not_pos727
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x730 = fmul double %mul_hi_abcd_max688, %mul_hi_abcd_max688
  %force_round_fr_mem731 = alloca double, align 8
  %194 = bitcast double* %force_round_fr_mem731 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %194)
  store volatile double %expt_even_neg_block_res_lo_unroll_x730, double* %force_round_fr_mem731, align 8
  %force_roundfr_rounded732 = load volatile double* %force_round_fr_mem731, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %194)
  %expt_even_neg_block_res_lo_unroll_x733 = fmul double %force_roundfr_rounded732, %force_roundfr_rounded732
  %force_round_fr_mem734 = alloca double, align 8
  %195 = bitcast double* %force_round_fr_mem734 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %195)
  store volatile double %expt_even_neg_block_res_lo_unroll_x733, double* %force_round_fr_mem734, align 8
  %force_roundfr_rounded735 = load volatile double* %force_round_fr_mem734, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %195)
  %force_round_fr_mem737 = alloca double, align 8
  %196 = bitcast double* %force_round_fr_mem737 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %196)
  store volatile double %force_roundfr_rounded735, double* %force_round_fr_mem737, align 8
  %force_roundfr_rounded738 = load volatile double* %force_round_fr_mem737, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %196)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x739 = fmul double %mul_lo_abcd_min706, %mul_lo_abcd_min706
  %force_round_fr_mem740 = alloca double, align 8
  %197 = bitcast double* %force_round_fr_mem740 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %197)
  store volatile double %expt_even_pos_block_res_hi_unroll_x739, double* %force_round_fr_mem740, align 8
  %force_roundfr_rounded741 = load volatile double* %force_round_fr_mem740, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %197)
  %expt_even_pos_block_res_hi_unroll_x742 = fmul double %force_roundfr_rounded741, %force_roundfr_rounded741
  %force_round_fr_mem743 = alloca double, align 8
  %198 = bitcast double* %force_round_fr_mem743 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %198)
  store volatile double %expt_even_pos_block_res_hi_unroll_x742, double* %force_round_fr_mem743, align 8
  %force_roundfr_rounded744 = load volatile double* %force_round_fr_mem743, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %198)
  %force_round_fr_mem746 = alloca double, align 8
  %199 = bitcast double* %force_round_fr_mem746 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %199)
  store volatile double %force_roundfr_rounded744, double* %force_round_fr_mem746, align 8
  %force_roundfr_rounded747 = load volatile double* %force_round_fr_mem746, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %199)
  br label %expt_even_merge769

expt_even_contains_zero748:                       ; preds = %expt_even_both_are_not_pos727
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x749 = fmul double %mul_lo_abcd_min706, %mul_lo_abcd_min706
  %force_round_fr_mem750 = alloca double, align 8
  %200 = bitcast double* %force_round_fr_mem750 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %200)
  store volatile double %expt_even_mid_block_lo__unroll_x749, double* %force_round_fr_mem750, align 8
  %force_roundfr_rounded751 = load volatile double* %force_round_fr_mem750, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %200)
  %expt_even_mid_block_lo__unroll_x752 = fmul double %force_roundfr_rounded751, %force_roundfr_rounded751
  %force_round_fr_mem753 = alloca double, align 8
  %201 = bitcast double* %force_round_fr_mem753 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %201)
  store volatile double %expt_even_mid_block_lo__unroll_x752, double* %force_round_fr_mem753, align 8
  %force_roundfr_rounded754 = load volatile double* %force_round_fr_mem753, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %201)
  %force_round_fr_mem756 = alloca double, align 8
  %202 = bitcast double* %force_round_fr_mem756 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %202)
  store volatile double %force_roundfr_rounded754, double* %force_round_fr_mem756, align 8
  %force_roundfr_rounded757 = load volatile double* %force_round_fr_mem756, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %202)
  %expt_even_mid_block_hi__unroll_x758 = fmul double %mul_hi_abcd_max688, %mul_hi_abcd_max688
  %force_round_fr_mem759 = alloca double, align 8
  %203 = bitcast double* %force_round_fr_mem759 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %203)
  store volatile double %expt_even_mid_block_hi__unroll_x758, double* %force_round_fr_mem759, align 8
  %force_roundfr_rounded760 = load volatile double* %force_round_fr_mem759, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %203)
  %expt_even_mid_block_hi__unroll_x761 = fmul double %force_roundfr_rounded760, %force_roundfr_rounded760
  %force_round_fr_mem762 = alloca double, align 8
  %204 = bitcast double* %force_round_fr_mem762 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %204)
  store volatile double %expt_even_mid_block_hi__unroll_x761, double* %force_round_fr_mem762, align 8
  %force_roundfr_rounded763 = load volatile double* %force_round_fr_mem762, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %204)
  %force_round_fr_mem765 = alloca double, align 8
  %205 = bitcast double* %force_round_fr_mem765 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %205)
  store volatile double %force_roundfr_rounded763, double* %force_round_fr_mem765, align 8
  %force_roundfr_rounded766 = load volatile double* %force_round_fr_mem765, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %205)
  %expt_even_mid_block_res_hi_abtest767 = fcmp olt double %force_roundfr_rounded757, %force_roundfr_rounded766
  %expt_even_mid_block_res_hi_abmax768 = select i1 %expt_even_mid_block_res_hi_abtest767, double %force_roundfr_rounded766, double %force_roundfr_rounded757
  br label %expt_even_merge769

expt_even_merge769:                               ; preds = %expt_even_contains_zero748, %expt_even_both_are_neg729, %expt_even_both_are_pos708
  %expt_lo770 = phi double [ %force_roundfr_rounded717, %expt_even_both_are_pos708 ], [ %force_roundfr_rounded738, %expt_even_both_are_neg729 ], [ 0.000000e+00, %expt_even_contains_zero748 ]
  %expt_hi771 = phi double [ %force_roundfr_rounded726, %expt_even_both_are_pos708 ], [ %force_roundfr_rounded747, %expt_even_both_are_neg729 ], [ %expt_even_mid_block_res_hi_abmax768, %expt_even_contains_zero748 ]
  call void @set_rounding_mode_down()
  %sub_lo772 = fsub double 5.000000e+01, %expt_hi771
  %force_round_fr_mem773 = alloca double, align 8
  %206 = bitcast double* %force_round_fr_mem773 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %206)
  store volatile double %sub_lo772, double* %force_round_fr_mem773, align 8
  %force_roundfr_rounded774 = load volatile double* %force_round_fr_mem773, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %206)
  call void @set_rounding_mode_up()
  %sub_hi775 = fsub double 5.000000e+01, %expt_lo770
  %force_round_fr_mem776 = alloca double, align 8
  %207 = bitcast double* %force_round_fr_mem776 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %207)
  store volatile double %sub_hi775, double* %force_round_fr_mem776, align 8
  %force_roundfr_rounded777 = load volatile double* %force_round_fr_mem776, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %207)
  %expt_even_are_both_pos_p778 = fcmp ogt double %force_roundfr_rounded774, 0.000000e+00
  br i1 %expt_even_are_both_pos_p778, label %expt_even_both_are_pos779, label %expt_even_both_are_not_pos798

expt_even_both_are_pos779:                        ; preds = %expt_even_merge769
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x780 = fmul double %force_roundfr_rounded774, %force_roundfr_rounded774
  %force_round_fr_mem781 = alloca double, align 8
  %208 = bitcast double* %force_round_fr_mem781 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %208)
  store volatile double %expt_even_pos_block_res_lo_unroll_x780, double* %force_round_fr_mem781, align 8
  %force_roundfr_rounded782 = load volatile double* %force_round_fr_mem781, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %208)
  %expt_even_pos_block_res_lo_unroll_x783 = fmul double %force_roundfr_rounded782, %force_roundfr_rounded782
  %force_round_fr_mem784 = alloca double, align 8
  %209 = bitcast double* %force_round_fr_mem784 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %209)
  store volatile double %expt_even_pos_block_res_lo_unroll_x783, double* %force_round_fr_mem784, align 8
  %force_roundfr_rounded785 = load volatile double* %force_round_fr_mem784, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %209)
  %force_round_fr_mem787 = alloca double, align 8
  %210 = bitcast double* %force_round_fr_mem787 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %210)
  store volatile double %force_roundfr_rounded785, double* %force_round_fr_mem787, align 8
  %force_roundfr_rounded788 = load volatile double* %force_round_fr_mem787, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %210)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x789 = fmul double %force_roundfr_rounded777, %force_roundfr_rounded777
  %force_round_fr_mem790 = alloca double, align 8
  %211 = bitcast double* %force_round_fr_mem790 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %211)
  store volatile double %expt_even_pos_block_res_hi_unroll_x789, double* %force_round_fr_mem790, align 8
  %force_roundfr_rounded791 = load volatile double* %force_round_fr_mem790, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %211)
  %expt_even_pos_block_res_hi_unroll_x792 = fmul double %force_roundfr_rounded791, %force_roundfr_rounded791
  %force_round_fr_mem793 = alloca double, align 8
  %212 = bitcast double* %force_round_fr_mem793 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %212)
  store volatile double %expt_even_pos_block_res_hi_unroll_x792, double* %force_round_fr_mem793, align 8
  %force_roundfr_rounded794 = load volatile double* %force_round_fr_mem793, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %212)
  %force_round_fr_mem796 = alloca double, align 8
  %213 = bitcast double* %force_round_fr_mem796 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %213)
  store volatile double %force_roundfr_rounded794, double* %force_round_fr_mem796, align 8
  %force_roundfr_rounded797 = load volatile double* %force_round_fr_mem796, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %213)
  br label %expt_even_merge840

expt_even_both_are_not_pos798:                    ; preds = %expt_even_merge769
  %expt_even_are_both_neg_p799 = fcmp olt double %force_roundfr_rounded777, 0.000000e+00
  br i1 %expt_even_are_both_neg_p799, label %expt_even_both_are_neg800, label %expt_even_contains_zero819

expt_even_both_are_neg800:                        ; preds = %expt_even_both_are_not_pos798
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x801 = fmul double %force_roundfr_rounded777, %force_roundfr_rounded777
  %force_round_fr_mem802 = alloca double, align 8
  %214 = bitcast double* %force_round_fr_mem802 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %214)
  store volatile double %expt_even_neg_block_res_lo_unroll_x801, double* %force_round_fr_mem802, align 8
  %force_roundfr_rounded803 = load volatile double* %force_round_fr_mem802, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %214)
  %expt_even_neg_block_res_lo_unroll_x804 = fmul double %force_roundfr_rounded803, %force_roundfr_rounded803
  %force_round_fr_mem805 = alloca double, align 8
  %215 = bitcast double* %force_round_fr_mem805 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %215)
  store volatile double %expt_even_neg_block_res_lo_unroll_x804, double* %force_round_fr_mem805, align 8
  %force_roundfr_rounded806 = load volatile double* %force_round_fr_mem805, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %215)
  %force_round_fr_mem808 = alloca double, align 8
  %216 = bitcast double* %force_round_fr_mem808 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %216)
  store volatile double %force_roundfr_rounded806, double* %force_round_fr_mem808, align 8
  %force_roundfr_rounded809 = load volatile double* %force_round_fr_mem808, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %216)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x810 = fmul double %force_roundfr_rounded774, %force_roundfr_rounded774
  %force_round_fr_mem811 = alloca double, align 8
  %217 = bitcast double* %force_round_fr_mem811 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %217)
  store volatile double %expt_even_pos_block_res_hi_unroll_x810, double* %force_round_fr_mem811, align 8
  %force_roundfr_rounded812 = load volatile double* %force_round_fr_mem811, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %217)
  %expt_even_pos_block_res_hi_unroll_x813 = fmul double %force_roundfr_rounded812, %force_roundfr_rounded812
  %force_round_fr_mem814 = alloca double, align 8
  %218 = bitcast double* %force_round_fr_mem814 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %218)
  store volatile double %expt_even_pos_block_res_hi_unroll_x813, double* %force_round_fr_mem814, align 8
  %force_roundfr_rounded815 = load volatile double* %force_round_fr_mem814, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %218)
  %force_round_fr_mem817 = alloca double, align 8
  %219 = bitcast double* %force_round_fr_mem817 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %219)
  store volatile double %force_roundfr_rounded815, double* %force_round_fr_mem817, align 8
  %force_roundfr_rounded818 = load volatile double* %force_round_fr_mem817, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %219)
  br label %expt_even_merge840

expt_even_contains_zero819:                       ; preds = %expt_even_both_are_not_pos798
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x820 = fmul double %force_roundfr_rounded774, %force_roundfr_rounded774
  %force_round_fr_mem821 = alloca double, align 8
  %220 = bitcast double* %force_round_fr_mem821 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %220)
  store volatile double %expt_even_mid_block_lo__unroll_x820, double* %force_round_fr_mem821, align 8
  %force_roundfr_rounded822 = load volatile double* %force_round_fr_mem821, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %220)
  %expt_even_mid_block_lo__unroll_x823 = fmul double %force_roundfr_rounded822, %force_roundfr_rounded822
  %force_round_fr_mem824 = alloca double, align 8
  %221 = bitcast double* %force_round_fr_mem824 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %221)
  store volatile double %expt_even_mid_block_lo__unroll_x823, double* %force_round_fr_mem824, align 8
  %force_roundfr_rounded825 = load volatile double* %force_round_fr_mem824, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %221)
  %force_round_fr_mem827 = alloca double, align 8
  %222 = bitcast double* %force_round_fr_mem827 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %222)
  store volatile double %force_roundfr_rounded825, double* %force_round_fr_mem827, align 8
  %force_roundfr_rounded828 = load volatile double* %force_round_fr_mem827, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %222)
  %expt_even_mid_block_hi__unroll_x829 = fmul double %force_roundfr_rounded777, %force_roundfr_rounded777
  %force_round_fr_mem830 = alloca double, align 8
  %223 = bitcast double* %force_round_fr_mem830 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %223)
  store volatile double %expt_even_mid_block_hi__unroll_x829, double* %force_round_fr_mem830, align 8
  %force_roundfr_rounded831 = load volatile double* %force_round_fr_mem830, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %223)
  %expt_even_mid_block_hi__unroll_x832 = fmul double %force_roundfr_rounded831, %force_roundfr_rounded831
  %force_round_fr_mem833 = alloca double, align 8
  %224 = bitcast double* %force_round_fr_mem833 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %224)
  store volatile double %expt_even_mid_block_hi__unroll_x832, double* %force_round_fr_mem833, align 8
  %force_roundfr_rounded834 = load volatile double* %force_round_fr_mem833, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %224)
  %force_round_fr_mem836 = alloca double, align 8
  %225 = bitcast double* %force_round_fr_mem836 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %225)
  store volatile double %force_roundfr_rounded834, double* %force_round_fr_mem836, align 8
  %force_roundfr_rounded837 = load volatile double* %force_round_fr_mem836, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %225)
  %expt_even_mid_block_res_hi_abtest838 = fcmp olt double %force_roundfr_rounded828, %force_roundfr_rounded837
  %expt_even_mid_block_res_hi_abmax839 = select i1 %expt_even_mid_block_res_hi_abtest838, double %force_roundfr_rounded837, double %force_roundfr_rounded828
  br label %expt_even_merge840

expt_even_merge840:                               ; preds = %expt_even_contains_zero819, %expt_even_both_are_neg800, %expt_even_both_are_pos779
  %expt_lo841 = phi double [ %force_roundfr_rounded788, %expt_even_both_are_pos779 ], [ %force_roundfr_rounded809, %expt_even_both_are_neg800 ], [ 0.000000e+00, %expt_even_contains_zero819 ]
  %expt_hi842 = phi double [ %force_roundfr_rounded797, %expt_even_both_are_pos779 ], [ %force_roundfr_rounded818, %expt_even_both_are_neg800 ], [ %expt_even_mid_block_res_hi_abmax839, %expt_even_contains_zero819 ]
  %div_hi_a843 = fdiv double %j_lo, %expt_lo841
  %force_round_fr_mem844 = alloca double, align 8
  %226 = bitcast double* %force_round_fr_mem844 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %226)
  store volatile double %div_hi_a843, double* %force_round_fr_mem844, align 8
  %force_roundfr_rounded845 = load volatile double* %force_round_fr_mem844, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %226)
  %div_hi_b846 = fdiv double %j_lo, %expt_hi842
  %force_round_fr_mem847 = alloca double, align 8
  %227 = bitcast double* %force_round_fr_mem847 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %227)
  store volatile double %div_hi_b846, double* %force_round_fr_mem847, align 8
  %force_roundfr_rounded848 = load volatile double* %force_round_fr_mem847, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %227)
  %div_hi_c849 = fdiv double %j_hi, %expt_lo841
  %force_round_fr_mem850 = alloca double, align 8
  %228 = bitcast double* %force_round_fr_mem850 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %228)
  store volatile double %div_hi_c849, double* %force_round_fr_mem850, align 8
  %force_roundfr_rounded851 = load volatile double* %force_round_fr_mem850, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %228)
  %div_hi_d852 = fdiv double %j_hi, %expt_hi842
  %force_round_fr_mem853 = alloca double, align 8
  %229 = bitcast double* %force_round_fr_mem853 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %229)
  store volatile double %div_hi_d852, double* %force_round_fr_mem853, align 8
  %force_roundfr_rounded854 = load volatile double* %force_round_fr_mem853, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %229)
  %div_hi_ab_test855 = fcmp olt double %force_roundfr_rounded845, %force_roundfr_rounded848
  %div_hi_cd_test856 = fcmp olt double %force_roundfr_rounded851, %force_roundfr_rounded854
  %div_hi_ab_max857 = select i1 %div_hi_ab_test855, double %force_roundfr_rounded848, double %force_roundfr_rounded845
  %div_hi_cd_max858 = select i1 %div_hi_cd_test856, double %force_roundfr_rounded854, double %force_roundfr_rounded851
  %div_hi_abcd_test859 = fcmp olt double %div_hi_ab_max857, %div_hi_cd_max858
  %div_hi_abcd_max860 = select i1 %div_hi_abcd_test859, double %div_hi_cd_max858, double %div_hi_ab_max857
  call void @set_rounding_mode_down()
  %force_round_fr_mem862 = alloca double, align 8
  %230 = bitcast double* %force_round_fr_mem862 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %230)
  store volatile double %div_hi_a843, double* %force_round_fr_mem862, align 8
  %force_roundfr_rounded863 = load volatile double* %force_round_fr_mem862, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %230)
  %force_round_fr_mem865 = alloca double, align 8
  %231 = bitcast double* %force_round_fr_mem865 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %231)
  store volatile double %div_hi_b846, double* %force_round_fr_mem865, align 8
  %force_roundfr_rounded866 = load volatile double* %force_round_fr_mem865, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %231)
  %force_round_fr_mem868 = alloca double, align 8
  %232 = bitcast double* %force_round_fr_mem868 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %232)
  store volatile double %div_hi_c849, double* %force_round_fr_mem868, align 8
  %force_roundfr_rounded869 = load volatile double* %force_round_fr_mem868, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %232)
  %force_round_fr_mem871 = alloca double, align 8
  %233 = bitcast double* %force_round_fr_mem871 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %233)
  store volatile double %div_hi_d852, double* %force_round_fr_mem871, align 8
  %force_roundfr_rounded872 = load volatile double* %force_round_fr_mem871, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %233)
  %div_lo_ab_test873 = fcmp olt double %force_roundfr_rounded866, %force_roundfr_rounded863
  %div_lo_cd_test874 = fcmp olt double %force_roundfr_rounded872, %force_roundfr_rounded869
  %div_lo_cd_min875 = select i1 %div_lo_cd_test874, double %force_roundfr_rounded872, double %force_roundfr_rounded869
  %div_lo_ab_min876 = select i1 %div_lo_ab_test873, double %force_roundfr_rounded866, double %force_roundfr_rounded863
  %div_lo_abcd_test877 = fcmp olt double %div_lo_cd_min875, %div_lo_ab_min876
  %div_lo_abcd_min878 = select i1 %div_lo_abcd_test877, double %div_lo_cd_min875, double %div_lo_ab_min876
  %sub_lo879 = fsub double 5.700000e+01, %div_hi_abcd_max860
  %force_round_fr_mem880 = alloca double, align 8
  %234 = bitcast double* %force_round_fr_mem880 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %234)
  store volatile double %sub_lo879, double* %force_round_fr_mem880, align 8
  %force_roundfr_rounded881 = load volatile double* %force_round_fr_mem880, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %234)
  call void @set_rounding_mode_up()
  %sub_hi882 = fsub double 5.700000e+01, %div_lo_abcd_min878
  %force_round_fr_mem883 = alloca double, align 8
  %235 = bitcast double* %force_round_fr_mem883 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %235)
  store volatile double %sub_hi882, double* %force_round_fr_mem883, align 8
  %force_roundfr_rounded884 = load volatile double* %force_round_fr_mem883, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %235)
  %mul_hi_a885 = fmul double %force_roundfr_rounded881, 6.300000e+01
  %force_round_fr_mem886 = alloca double, align 8
  %236 = bitcast double* %force_round_fr_mem886 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %236)
  store volatile double %mul_hi_a885, double* %force_round_fr_mem886, align 8
  %force_roundfr_rounded887 = load volatile double* %force_round_fr_mem886, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %236)
  %force_round_fr_mem889 = alloca double, align 8
  %237 = bitcast double* %force_round_fr_mem889 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %237)
  store volatile double %mul_hi_a885, double* %force_round_fr_mem889, align 8
  %force_roundfr_rounded890 = load volatile double* %force_round_fr_mem889, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %237)
  %mul_hi_c891 = fmul double %force_roundfr_rounded884, 6.300000e+01
  %force_round_fr_mem892 = alloca double, align 8
  %238 = bitcast double* %force_round_fr_mem892 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %238)
  store volatile double %mul_hi_c891, double* %force_round_fr_mem892, align 8
  %force_roundfr_rounded893 = load volatile double* %force_round_fr_mem892, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %238)
  %force_round_fr_mem895 = alloca double, align 8
  %239 = bitcast double* %force_round_fr_mem895 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %239)
  store volatile double %mul_hi_c891, double* %force_round_fr_mem895, align 8
  %force_roundfr_rounded896 = load volatile double* %force_round_fr_mem895, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %239)
  %mul_hi_ab_test897 = fcmp olt double %force_roundfr_rounded887, %force_roundfr_rounded890
  %mul_hi_cd_test898 = fcmp olt double %force_roundfr_rounded893, %force_roundfr_rounded896
  %mul_hi_ab_max899 = select i1 %mul_hi_ab_test897, double %force_roundfr_rounded890, double %force_roundfr_rounded887
  %mul_hi_cd_max900 = select i1 %mul_hi_cd_test898, double %force_roundfr_rounded896, double %force_roundfr_rounded893
  %mul_hi_abcd_test901 = fcmp olt double %mul_hi_ab_max899, %mul_hi_cd_max900
  %mul_hi_abcd_max902 = select i1 %mul_hi_abcd_test901, double %mul_hi_cd_max900, double %mul_hi_ab_max899
  call void @set_rounding_mode_down()
  %force_round_fr_mem904 = alloca double, align 8
  %240 = bitcast double* %force_round_fr_mem904 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %240)
  store volatile double %mul_hi_a885, double* %force_round_fr_mem904, align 8
  %force_roundfr_rounded905 = load volatile double* %force_round_fr_mem904, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %240)
  %force_round_fr_mem907 = alloca double, align 8
  %241 = bitcast double* %force_round_fr_mem907 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %241)
  store volatile double %mul_hi_a885, double* %force_round_fr_mem907, align 8
  %force_roundfr_rounded908 = load volatile double* %force_round_fr_mem907, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %241)
  %force_round_fr_mem910 = alloca double, align 8
  %242 = bitcast double* %force_round_fr_mem910 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %242)
  store volatile double %mul_hi_c891, double* %force_round_fr_mem910, align 8
  %force_roundfr_rounded911 = load volatile double* %force_round_fr_mem910, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %242)
  %force_round_fr_mem913 = alloca double, align 8
  %243 = bitcast double* %force_round_fr_mem913 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %243)
  store volatile double %mul_hi_c891, double* %force_round_fr_mem913, align 8
  %force_roundfr_rounded914 = load volatile double* %force_round_fr_mem913, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %243)
  %mul_lo_ab_test915 = fcmp olt double %force_roundfr_rounded908, %force_roundfr_rounded905
  %mul_lo_cd_test916 = fcmp olt double %force_roundfr_rounded914, %force_roundfr_rounded911
  %mul_lo_cd_min917 = select i1 %mul_lo_cd_test916, double %force_roundfr_rounded914, double %force_roundfr_rounded911
  %mul_lo_ab_min918 = select i1 %mul_lo_ab_test915, double %force_roundfr_rounded908, double %force_roundfr_rounded905
  %mul_lo_abcd_test919 = fcmp olt double %mul_lo_cd_min917, %mul_lo_ab_min918
  %mul_lo_abcd_min920 = select i1 %mul_lo_abcd_test919, double %mul_lo_cd_min917, double %mul_lo_ab_min918
  call void @set_rounding_mode_up()
  %div_hi_a921 = fdiv double %mul_lo_abcd_min920, 4.400000e+01
  %force_round_fr_mem922 = alloca double, align 8
  %244 = bitcast double* %force_round_fr_mem922 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %244)
  store volatile double %div_hi_a921, double* %force_round_fr_mem922, align 8
  %force_roundfr_rounded923 = load volatile double* %force_round_fr_mem922, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %244)
  %force_round_fr_mem925 = alloca double, align 8
  %245 = bitcast double* %force_round_fr_mem925 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %245)
  store volatile double %div_hi_a921, double* %force_round_fr_mem925, align 8
  %force_roundfr_rounded926 = load volatile double* %force_round_fr_mem925, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %245)
  %div_hi_c927 = fdiv double %mul_hi_abcd_max902, 4.400000e+01
  %force_round_fr_mem928 = alloca double, align 8
  %246 = bitcast double* %force_round_fr_mem928 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %246)
  store volatile double %div_hi_c927, double* %force_round_fr_mem928, align 8
  %force_roundfr_rounded929 = load volatile double* %force_round_fr_mem928, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %246)
  %force_round_fr_mem931 = alloca double, align 8
  %247 = bitcast double* %force_round_fr_mem931 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %247)
  store volatile double %div_hi_c927, double* %force_round_fr_mem931, align 8
  %force_roundfr_rounded932 = load volatile double* %force_round_fr_mem931, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %247)
  %div_hi_ab_test933 = fcmp olt double %force_roundfr_rounded923, %force_roundfr_rounded926
  %div_hi_cd_test934 = fcmp olt double %force_roundfr_rounded929, %force_roundfr_rounded932
  %div_hi_ab_max935 = select i1 %div_hi_ab_test933, double %force_roundfr_rounded926, double %force_roundfr_rounded923
  %div_hi_cd_max936 = select i1 %div_hi_cd_test934, double %force_roundfr_rounded932, double %force_roundfr_rounded929
  %div_hi_abcd_test937 = fcmp olt double %div_hi_ab_max935, %div_hi_cd_max936
  %div_hi_abcd_max938 = select i1 %div_hi_abcd_test937, double %div_hi_cd_max936, double %div_hi_ab_max935
  call void @set_rounding_mode_down()
  %force_round_fr_mem940 = alloca double, align 8
  %248 = bitcast double* %force_round_fr_mem940 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %248)
  store volatile double %div_hi_a921, double* %force_round_fr_mem940, align 8
  %force_roundfr_rounded941 = load volatile double* %force_round_fr_mem940, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %248)
  %force_round_fr_mem943 = alloca double, align 8
  %249 = bitcast double* %force_round_fr_mem943 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %249)
  store volatile double %div_hi_a921, double* %force_round_fr_mem943, align 8
  %force_roundfr_rounded944 = load volatile double* %force_round_fr_mem943, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %249)
  %force_round_fr_mem946 = alloca double, align 8
  %250 = bitcast double* %force_round_fr_mem946 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %250)
  store volatile double %div_hi_c927, double* %force_round_fr_mem946, align 8
  %force_roundfr_rounded947 = load volatile double* %force_round_fr_mem946, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %250)
  %force_round_fr_mem949 = alloca double, align 8
  %251 = bitcast double* %force_round_fr_mem949 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %251)
  store volatile double %div_hi_c927, double* %force_round_fr_mem949, align 8
  %force_roundfr_rounded950 = load volatile double* %force_round_fr_mem949, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %251)
  %div_lo_ab_test951 = fcmp olt double %force_roundfr_rounded944, %force_roundfr_rounded941
  %div_lo_cd_test952 = fcmp olt double %force_roundfr_rounded950, %force_roundfr_rounded947
  %div_lo_cd_min953 = select i1 %div_lo_cd_test952, double %force_roundfr_rounded950, double %force_roundfr_rounded947
  %div_lo_ab_min954 = select i1 %div_lo_ab_test951, double %force_roundfr_rounded944, double %force_roundfr_rounded941
  %div_lo_abcd_test955 = fcmp olt double %div_lo_cd_min953, %div_lo_ab_min954
  %div_lo_abcd_min956 = select i1 %div_lo_abcd_test955, double %div_lo_cd_min953, double %div_lo_ab_min954
  %add_lo957 = fadd double %div_lo_abcd_min956, %k_lo
  %force_round_fr_mem958 = alloca double, align 8
  %252 = bitcast double* %force_round_fr_mem958 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %252)
  store volatile double %add_lo957, double* %force_round_fr_mem958, align 8
  %force_roundfr_rounded959 = load volatile double* %force_round_fr_mem958, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %252)
  call void @set_rounding_mode_up()
  %add_hi960 = fadd double %div_hi_abcd_max938, %k_hi
  %force_round_fr_mem961 = alloca double, align 8
  %253 = bitcast double* %force_round_fr_mem961 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %253)
  store volatile double %add_hi960, double* %force_round_fr_mem961, align 8
  %force_roundfr_rounded962 = load volatile double* %force_round_fr_mem961, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %253)
  %mul_hi_a963 = fmul double %force_roundfr_rounded959, %l_lo
  %force_round_fr_mem964 = alloca double, align 8
  %254 = bitcast double* %force_round_fr_mem964 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %254)
  store volatile double %mul_hi_a963, double* %force_round_fr_mem964, align 8
  %force_roundfr_rounded965 = load volatile double* %force_round_fr_mem964, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %254)
  %mul_hi_b966 = fmul double %force_roundfr_rounded959, %l_hi
  %force_round_fr_mem967 = alloca double, align 8
  %255 = bitcast double* %force_round_fr_mem967 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %255)
  store volatile double %mul_hi_b966, double* %force_round_fr_mem967, align 8
  %force_roundfr_rounded968 = load volatile double* %force_round_fr_mem967, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %255)
  %mul_hi_c969 = fmul double %force_roundfr_rounded962, %l_lo
  %force_round_fr_mem970 = alloca double, align 8
  %256 = bitcast double* %force_round_fr_mem970 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %256)
  store volatile double %mul_hi_c969, double* %force_round_fr_mem970, align 8
  %force_roundfr_rounded971 = load volatile double* %force_round_fr_mem970, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %256)
  %mul_hi_d972 = fmul double %force_roundfr_rounded962, %l_hi
  %force_round_fr_mem973 = alloca double, align 8
  %257 = bitcast double* %force_round_fr_mem973 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %257)
  store volatile double %mul_hi_d972, double* %force_round_fr_mem973, align 8
  %force_roundfr_rounded974 = load volatile double* %force_round_fr_mem973, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %257)
  %mul_hi_ab_test975 = fcmp olt double %force_roundfr_rounded965, %force_roundfr_rounded968
  %mul_hi_cd_test976 = fcmp olt double %force_roundfr_rounded971, %force_roundfr_rounded974
  %mul_hi_ab_max977 = select i1 %mul_hi_ab_test975, double %force_roundfr_rounded968, double %force_roundfr_rounded965
  %mul_hi_cd_max978 = select i1 %mul_hi_cd_test976, double %force_roundfr_rounded974, double %force_roundfr_rounded971
  %mul_hi_abcd_test979 = fcmp olt double %mul_hi_ab_max977, %mul_hi_cd_max978
  %mul_hi_abcd_max980 = select i1 %mul_hi_abcd_test979, double %mul_hi_cd_max978, double %mul_hi_ab_max977
  call void @set_rounding_mode_down()
  %force_round_fr_mem982 = alloca double, align 8
  %258 = bitcast double* %force_round_fr_mem982 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %258)
  store volatile double %mul_hi_a963, double* %force_round_fr_mem982, align 8
  %force_roundfr_rounded983 = load volatile double* %force_round_fr_mem982, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %258)
  %force_round_fr_mem985 = alloca double, align 8
  %259 = bitcast double* %force_round_fr_mem985 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %259)
  store volatile double %mul_hi_b966, double* %force_round_fr_mem985, align 8
  %force_roundfr_rounded986 = load volatile double* %force_round_fr_mem985, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %259)
  %force_round_fr_mem988 = alloca double, align 8
  %260 = bitcast double* %force_round_fr_mem988 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %260)
  store volatile double %mul_hi_c969, double* %force_round_fr_mem988, align 8
  %force_roundfr_rounded989 = load volatile double* %force_round_fr_mem988, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %260)
  %force_round_fr_mem991 = alloca double, align 8
  %261 = bitcast double* %force_round_fr_mem991 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %261)
  store volatile double %mul_hi_d972, double* %force_round_fr_mem991, align 8
  %force_roundfr_rounded992 = load volatile double* %force_round_fr_mem991, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %261)
  %mul_lo_ab_test993 = fcmp olt double %force_roundfr_rounded986, %force_roundfr_rounded983
  %mul_lo_cd_test994 = fcmp olt double %force_roundfr_rounded992, %force_roundfr_rounded989
  %mul_lo_cd_min995 = select i1 %mul_lo_cd_test994, double %force_roundfr_rounded992, double %force_roundfr_rounded989
  %mul_lo_ab_min996 = select i1 %mul_lo_ab_test993, double %force_roundfr_rounded986, double %force_roundfr_rounded983
  %mul_lo_abcd_test997 = fcmp olt double %mul_lo_cd_min995, %mul_lo_ab_min996
  %mul_lo_abcd_min998 = select i1 %mul_lo_abcd_test997, double %mul_lo_cd_min995, double %mul_lo_ab_min996
  call void @set_rounding_mode_up()
  %mul_hi_a999 = fmul double %mul_lo_abcd_min998, %m_lo
  %force_round_fr_mem1000 = alloca double, align 8
  %262 = bitcast double* %force_round_fr_mem1000 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %262)
  store volatile double %mul_hi_a999, double* %force_round_fr_mem1000, align 8
  %force_roundfr_rounded1001 = load volatile double* %force_round_fr_mem1000, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %262)
  %mul_hi_b1002 = fmul double %mul_hi_abcd_max980, %m_lo
  %force_round_fr_mem1003 = alloca double, align 8
  %263 = bitcast double* %force_round_fr_mem1003 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %263)
  store volatile double %mul_hi_b1002, double* %force_round_fr_mem1003, align 8
  %force_roundfr_rounded1004 = load volatile double* %force_round_fr_mem1003, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %263)
  %mul_hi_c1005 = fmul double %mul_lo_abcd_min998, %m_hi
  %force_round_fr_mem1006 = alloca double, align 8
  %264 = bitcast double* %force_round_fr_mem1006 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %264)
  store volatile double %mul_hi_c1005, double* %force_round_fr_mem1006, align 8
  %force_roundfr_rounded1007 = load volatile double* %force_round_fr_mem1006, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %264)
  %mul_hi_d1008 = fmul double %mul_hi_abcd_max980, %m_hi
  %force_round_fr_mem1009 = alloca double, align 8
  %265 = bitcast double* %force_round_fr_mem1009 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %265)
  store volatile double %mul_hi_d1008, double* %force_round_fr_mem1009, align 8
  %force_roundfr_rounded1010 = load volatile double* %force_round_fr_mem1009, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %265)
  %mul_hi_ab_test1011 = fcmp olt double %force_roundfr_rounded1001, %force_roundfr_rounded1004
  %mul_hi_cd_test1012 = fcmp olt double %force_roundfr_rounded1007, %force_roundfr_rounded1010
  %mul_hi_ab_max1013 = select i1 %mul_hi_ab_test1011, double %force_roundfr_rounded1004, double %force_roundfr_rounded1001
  %mul_hi_cd_max1014 = select i1 %mul_hi_cd_test1012, double %force_roundfr_rounded1010, double %force_roundfr_rounded1007
  %mul_hi_abcd_test1015 = fcmp olt double %mul_hi_ab_max1013, %mul_hi_cd_max1014
  %mul_hi_abcd_max1016 = select i1 %mul_hi_abcd_test1015, double %mul_hi_cd_max1014, double %mul_hi_ab_max1013
  call void @set_rounding_mode_down()
  %force_round_fr_mem1018 = alloca double, align 8
  %266 = bitcast double* %force_round_fr_mem1018 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %266)
  store volatile double %mul_hi_a999, double* %force_round_fr_mem1018, align 8
  %force_roundfr_rounded1019 = load volatile double* %force_round_fr_mem1018, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %266)
  %force_round_fr_mem1021 = alloca double, align 8
  %267 = bitcast double* %force_round_fr_mem1021 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %267)
  store volatile double %mul_hi_b1002, double* %force_round_fr_mem1021, align 8
  %force_roundfr_rounded1022 = load volatile double* %force_round_fr_mem1021, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %267)
  %force_round_fr_mem1024 = alloca double, align 8
  %268 = bitcast double* %force_round_fr_mem1024 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %268)
  store volatile double %mul_hi_c1005, double* %force_round_fr_mem1024, align 8
  %force_roundfr_rounded1025 = load volatile double* %force_round_fr_mem1024, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %268)
  %force_round_fr_mem1027 = alloca double, align 8
  %269 = bitcast double* %force_round_fr_mem1027 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %269)
  store volatile double %mul_hi_d1008, double* %force_round_fr_mem1027, align 8
  %force_roundfr_rounded1028 = load volatile double* %force_round_fr_mem1027, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %269)
  %mul_lo_ab_test1029 = fcmp olt double %force_roundfr_rounded1022, %force_roundfr_rounded1019
  %mul_lo_cd_test1030 = fcmp olt double %force_roundfr_rounded1028, %force_roundfr_rounded1025
  %mul_lo_cd_min1031 = select i1 %mul_lo_cd_test1030, double %force_roundfr_rounded1028, double %force_roundfr_rounded1025
  %mul_lo_ab_min1032 = select i1 %mul_lo_ab_test1029, double %force_roundfr_rounded1022, double %force_roundfr_rounded1019
  %mul_lo_abcd_test1033 = fcmp olt double %mul_lo_cd_min1031, %mul_lo_ab_min1032
  %mul_lo_abcd_min1034 = select i1 %mul_lo_abcd_test1033, double %mul_lo_cd_min1031, double %mul_lo_ab_min1032
  %neg_lo1035 = fsub double -0.000000e+00, %mul_hi_abcd_max1016
  %neg_hi1036 = fsub double -0.000000e+00, %mul_lo_abcd_min1034
  %expt_odd_lo_unroll_x1037 = fmul double %mul_hi_abcd_max1016, %mul_hi_abcd_max1016
  %force_round_fr_mem1038 = alloca double, align 8
  %270 = bitcast double* %force_round_fr_mem1038 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %270)
  store volatile double %expt_odd_lo_unroll_x1037, double* %force_round_fr_mem1038, align 8
  %force_roundfr_rounded1039 = load volatile double* %force_round_fr_mem1038, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %270)
  %expt_odd_lopwr_do_y1040 = fmul double %force_roundfr_rounded1039, %neg_lo1035
  %force_round_fr_mem1041 = alloca double, align 8
  %271 = bitcast double* %force_round_fr_mem1041 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %271)
  store volatile double %expt_odd_lopwr_do_y1040, double* %force_round_fr_mem1041, align 8
  %force_roundfr_rounded1042 = load volatile double* %force_round_fr_mem1041, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %271)
  call void @set_rounding_mode_up()
  %expt_odd_hi_unroll_x1043 = fmul double %mul_lo_abcd_min1034, %mul_lo_abcd_min1034
  %force_round_fr_mem1044 = alloca double, align 8
  %272 = bitcast double* %force_round_fr_mem1044 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %272)
  store volatile double %expt_odd_hi_unroll_x1043, double* %force_round_fr_mem1044, align 8
  %force_roundfr_rounded1045 = load volatile double* %force_round_fr_mem1044, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %272)
  %expt_odd_hipwr_do_y1046 = fmul double %force_roundfr_rounded1045, %neg_hi1036
  %force_round_fr_mem1047 = alloca double, align 8
  %273 = bitcast double* %force_round_fr_mem1047 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %273)
  store volatile double %expt_odd_hipwr_do_y1046, double* %force_round_fr_mem1047, align 8
  %force_roundfr_rounded1048 = load volatile double* %force_round_fr_mem1047, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %273)
  %expt_even_are_both_pos_p1049 = fcmp ogt double %force_roundfr_rounded1042, 0.000000e+00
  br i1 %expt_even_are_both_pos_p1049, label %expt_even_both_are_pos1050, label %expt_even_both_are_not_pos1063

expt_even_both_are_pos1050:                       ; preds = %expt_even_merge840
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x1051 = fmul double %force_roundfr_rounded1042, %force_roundfr_rounded1042
  %force_round_fr_mem1052 = alloca double, align 8
  %274 = bitcast double* %force_round_fr_mem1052 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %274)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1051, double* %force_round_fr_mem1052, align 8
  %force_roundfr_rounded1053 = load volatile double* %force_round_fr_mem1052, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %274)
  %force_round_fr_mem1055 = alloca double, align 8
  %275 = bitcast double* %force_round_fr_mem1055 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %275)
  store volatile double %force_roundfr_rounded1053, double* %force_round_fr_mem1055, align 8
  %force_roundfr_rounded1056 = load volatile double* %force_round_fr_mem1055, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %275)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1057 = fmul double %force_roundfr_rounded1048, %force_roundfr_rounded1048
  %force_round_fr_mem1058 = alloca double, align 8
  %276 = bitcast double* %force_round_fr_mem1058 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %276)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1057, double* %force_round_fr_mem1058, align 8
  %force_roundfr_rounded1059 = load volatile double* %force_round_fr_mem1058, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %276)
  %force_round_fr_mem1061 = alloca double, align 8
  %277 = bitcast double* %force_round_fr_mem1061 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %277)
  store volatile double %force_roundfr_rounded1059, double* %force_round_fr_mem1061, align 8
  %force_roundfr_rounded1062 = load volatile double* %force_round_fr_mem1061, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %277)
  br label %expt_even_merge1093

expt_even_both_are_not_pos1063:                   ; preds = %expt_even_merge840
  %expt_even_are_both_neg_p1064 = fcmp olt double %force_roundfr_rounded1048, 0.000000e+00
  br i1 %expt_even_are_both_neg_p1064, label %expt_even_both_are_neg1065, label %expt_even_contains_zero1078

expt_even_both_are_neg1065:                       ; preds = %expt_even_both_are_not_pos1063
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x1066 = fmul double %force_roundfr_rounded1048, %force_roundfr_rounded1048
  %force_round_fr_mem1067 = alloca double, align 8
  %278 = bitcast double* %force_round_fr_mem1067 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %278)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1066, double* %force_round_fr_mem1067, align 8
  %force_roundfr_rounded1068 = load volatile double* %force_round_fr_mem1067, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %278)
  %force_round_fr_mem1070 = alloca double, align 8
  %279 = bitcast double* %force_round_fr_mem1070 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %279)
  store volatile double %force_roundfr_rounded1068, double* %force_round_fr_mem1070, align 8
  %force_roundfr_rounded1071 = load volatile double* %force_round_fr_mem1070, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %279)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1072 = fmul double %force_roundfr_rounded1042, %force_roundfr_rounded1042
  %force_round_fr_mem1073 = alloca double, align 8
  %280 = bitcast double* %force_round_fr_mem1073 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %280)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1072, double* %force_round_fr_mem1073, align 8
  %force_roundfr_rounded1074 = load volatile double* %force_round_fr_mem1073, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %280)
  %force_round_fr_mem1076 = alloca double, align 8
  %281 = bitcast double* %force_round_fr_mem1076 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %281)
  store volatile double %force_roundfr_rounded1074, double* %force_round_fr_mem1076, align 8
  %force_roundfr_rounded1077 = load volatile double* %force_round_fr_mem1076, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %281)
  br label %expt_even_merge1093

expt_even_contains_zero1078:                      ; preds = %expt_even_both_are_not_pos1063
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x1079 = fmul double %force_roundfr_rounded1042, %force_roundfr_rounded1042
  %force_round_fr_mem1080 = alloca double, align 8
  %282 = bitcast double* %force_round_fr_mem1080 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %282)
  store volatile double %expt_even_mid_block_lo__unroll_x1079, double* %force_round_fr_mem1080, align 8
  %force_roundfr_rounded1081 = load volatile double* %force_round_fr_mem1080, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %282)
  %force_round_fr_mem1083 = alloca double, align 8
  %283 = bitcast double* %force_round_fr_mem1083 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %283)
  store volatile double %force_roundfr_rounded1081, double* %force_round_fr_mem1083, align 8
  %force_roundfr_rounded1084 = load volatile double* %force_round_fr_mem1083, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %283)
  %expt_even_mid_block_hi__unroll_x1085 = fmul double %force_roundfr_rounded1048, %force_roundfr_rounded1048
  %force_round_fr_mem1086 = alloca double, align 8
  %284 = bitcast double* %force_round_fr_mem1086 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %284)
  store volatile double %expt_even_mid_block_hi__unroll_x1085, double* %force_round_fr_mem1086, align 8
  %force_roundfr_rounded1087 = load volatile double* %force_round_fr_mem1086, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %284)
  %force_round_fr_mem1089 = alloca double, align 8
  %285 = bitcast double* %force_round_fr_mem1089 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %285)
  store volatile double %force_roundfr_rounded1087, double* %force_round_fr_mem1089, align 8
  %force_roundfr_rounded1090 = load volatile double* %force_round_fr_mem1089, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %285)
  %expt_even_mid_block_res_hi_abtest1091 = fcmp olt double %force_roundfr_rounded1084, %force_roundfr_rounded1090
  %expt_even_mid_block_res_hi_abmax1092 = select i1 %expt_even_mid_block_res_hi_abtest1091, double %force_roundfr_rounded1090, double %force_roundfr_rounded1084
  br label %expt_even_merge1093

expt_even_merge1093:                              ; preds = %expt_even_contains_zero1078, %expt_even_both_are_neg1065, %expt_even_both_are_pos1050
  %expt_lo1094 = phi double [ %force_roundfr_rounded1056, %expt_even_both_are_pos1050 ], [ %force_roundfr_rounded1071, %expt_even_both_are_neg1065 ], [ 0.000000e+00, %expt_even_contains_zero1078 ]
  %expt_hi1095 = phi double [ %force_roundfr_rounded1062, %expt_even_both_are_pos1050 ], [ %force_roundfr_rounded1077, %expt_even_both_are_neg1065 ], [ %expt_even_mid_block_res_hi_abmax1092, %expt_even_contains_zero1078 ]
  %mul_hi_a1096 = fmul double %expt_lo1094, -1.700000e+01
  %force_round_fr_mem1097 = alloca double, align 8
  %286 = bitcast double* %force_round_fr_mem1097 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %286)
  store volatile double %mul_hi_a1096, double* %force_round_fr_mem1097, align 8
  %force_roundfr_rounded1098 = load volatile double* %force_round_fr_mem1097, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %286)
  %mul_hi_b1099 = fmul double %expt_hi1095, -1.700000e+01
  %force_round_fr_mem1100 = alloca double, align 8
  %287 = bitcast double* %force_round_fr_mem1100 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %287)
  store volatile double %mul_hi_b1099, double* %force_round_fr_mem1100, align 8
  %force_roundfr_rounded1101 = load volatile double* %force_round_fr_mem1100, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %287)
  %force_round_fr_mem1103 = alloca double, align 8
  %288 = bitcast double* %force_round_fr_mem1103 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %288)
  store volatile double %mul_hi_a1096, double* %force_round_fr_mem1103, align 8
  %force_roundfr_rounded1104 = load volatile double* %force_round_fr_mem1103, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %288)
  %force_round_fr_mem1106 = alloca double, align 8
  %289 = bitcast double* %force_round_fr_mem1106 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %289)
  store volatile double %mul_hi_b1099, double* %force_round_fr_mem1106, align 8
  %force_roundfr_rounded1107 = load volatile double* %force_round_fr_mem1106, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %289)
  %mul_hi_ab_test1108 = fcmp olt double %force_roundfr_rounded1098, %force_roundfr_rounded1101
  %mul_hi_cd_test1109 = fcmp olt double %force_roundfr_rounded1104, %force_roundfr_rounded1107
  %mul_hi_ab_max1110 = select i1 %mul_hi_ab_test1108, double %force_roundfr_rounded1101, double %force_roundfr_rounded1098
  %mul_hi_cd_max1111 = select i1 %mul_hi_cd_test1109, double %force_roundfr_rounded1107, double %force_roundfr_rounded1104
  %mul_hi_abcd_test1112 = fcmp olt double %mul_hi_ab_max1110, %mul_hi_cd_max1111
  %mul_hi_abcd_max1113 = select i1 %mul_hi_abcd_test1112, double %mul_hi_cd_max1111, double %mul_hi_ab_max1110
  call void @set_rounding_mode_down()
  %force_round_fr_mem1115 = alloca double, align 8
  %290 = bitcast double* %force_round_fr_mem1115 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %290)
  store volatile double %mul_hi_a1096, double* %force_round_fr_mem1115, align 8
  %force_roundfr_rounded1116 = load volatile double* %force_round_fr_mem1115, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %290)
  %force_round_fr_mem1118 = alloca double, align 8
  %291 = bitcast double* %force_round_fr_mem1118 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %291)
  store volatile double %mul_hi_b1099, double* %force_round_fr_mem1118, align 8
  %force_roundfr_rounded1119 = load volatile double* %force_round_fr_mem1118, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %291)
  %force_round_fr_mem1121 = alloca double, align 8
  %292 = bitcast double* %force_round_fr_mem1121 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %292)
  store volatile double %mul_hi_a1096, double* %force_round_fr_mem1121, align 8
  %force_roundfr_rounded1122 = load volatile double* %force_round_fr_mem1121, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %292)
  %force_round_fr_mem1124 = alloca double, align 8
  %293 = bitcast double* %force_round_fr_mem1124 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %293)
  store volatile double %mul_hi_b1099, double* %force_round_fr_mem1124, align 8
  %force_roundfr_rounded1125 = load volatile double* %force_round_fr_mem1124, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %293)
  %mul_lo_ab_test1126 = fcmp olt double %force_roundfr_rounded1119, %force_roundfr_rounded1116
  %mul_lo_cd_test1127 = fcmp olt double %force_roundfr_rounded1125, %force_roundfr_rounded1122
  %mul_lo_cd_min1128 = select i1 %mul_lo_cd_test1127, double %force_roundfr_rounded1125, double %force_roundfr_rounded1122
  %mul_lo_ab_min1129 = select i1 %mul_lo_ab_test1126, double %force_roundfr_rounded1119, double %force_roundfr_rounded1116
  %mul_lo_abcd_test1130 = fcmp olt double %mul_lo_cd_min1128, %mul_lo_ab_min1129
  %mul_lo_abcd_min1131 = select i1 %mul_lo_abcd_test1130, double %mul_lo_cd_min1128, double %mul_lo_ab_min1129
  call void @set_rounding_mode_up()
  %mul_hi_a1132 = fmul double %mul_lo_abcd_min1131, %n_lo
  %force_round_fr_mem1133 = alloca double, align 8
  %294 = bitcast double* %force_round_fr_mem1133 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %294)
  store volatile double %mul_hi_a1132, double* %force_round_fr_mem1133, align 8
  %force_roundfr_rounded1134 = load volatile double* %force_round_fr_mem1133, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %294)
  %mul_hi_b1135 = fmul double %mul_lo_abcd_min1131, %n_hi
  %force_round_fr_mem1136 = alloca double, align 8
  %295 = bitcast double* %force_round_fr_mem1136 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %295)
  store volatile double %mul_hi_b1135, double* %force_round_fr_mem1136, align 8
  %force_roundfr_rounded1137 = load volatile double* %force_round_fr_mem1136, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %295)
  %mul_hi_c1138 = fmul double %mul_hi_abcd_max1113, %n_lo
  %force_round_fr_mem1139 = alloca double, align 8
  %296 = bitcast double* %force_round_fr_mem1139 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %296)
  store volatile double %mul_hi_c1138, double* %force_round_fr_mem1139, align 8
  %force_roundfr_rounded1140 = load volatile double* %force_round_fr_mem1139, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %296)
  %mul_hi_d1141 = fmul double %mul_hi_abcd_max1113, %n_hi
  %force_round_fr_mem1142 = alloca double, align 8
  %297 = bitcast double* %force_round_fr_mem1142 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %297)
  store volatile double %mul_hi_d1141, double* %force_round_fr_mem1142, align 8
  %force_roundfr_rounded1143 = load volatile double* %force_round_fr_mem1142, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %297)
  %mul_hi_ab_test1144 = fcmp olt double %force_roundfr_rounded1134, %force_roundfr_rounded1137
  %mul_hi_cd_test1145 = fcmp olt double %force_roundfr_rounded1140, %force_roundfr_rounded1143
  %mul_hi_ab_max1146 = select i1 %mul_hi_ab_test1144, double %force_roundfr_rounded1137, double %force_roundfr_rounded1134
  %mul_hi_cd_max1147 = select i1 %mul_hi_cd_test1145, double %force_roundfr_rounded1143, double %force_roundfr_rounded1140
  %mul_hi_abcd_test1148 = fcmp olt double %mul_hi_ab_max1146, %mul_hi_cd_max1147
  %mul_hi_abcd_max1149 = select i1 %mul_hi_abcd_test1148, double %mul_hi_cd_max1147, double %mul_hi_ab_max1146
  call void @set_rounding_mode_down()
  %force_round_fr_mem1151 = alloca double, align 8
  %298 = bitcast double* %force_round_fr_mem1151 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %298)
  store volatile double %mul_hi_a1132, double* %force_round_fr_mem1151, align 8
  %force_roundfr_rounded1152 = load volatile double* %force_round_fr_mem1151, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %298)
  %force_round_fr_mem1154 = alloca double, align 8
  %299 = bitcast double* %force_round_fr_mem1154 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %299)
  store volatile double %mul_hi_b1135, double* %force_round_fr_mem1154, align 8
  %force_roundfr_rounded1155 = load volatile double* %force_round_fr_mem1154, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %299)
  %force_round_fr_mem1157 = alloca double, align 8
  %300 = bitcast double* %force_round_fr_mem1157 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %300)
  store volatile double %mul_hi_c1138, double* %force_round_fr_mem1157, align 8
  %force_roundfr_rounded1158 = load volatile double* %force_round_fr_mem1157, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %300)
  %force_round_fr_mem1160 = alloca double, align 8
  %301 = bitcast double* %force_round_fr_mem1160 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %301)
  store volatile double %mul_hi_d1141, double* %force_round_fr_mem1160, align 8
  %force_roundfr_rounded1161 = load volatile double* %force_round_fr_mem1160, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %301)
  %mul_lo_ab_test1162 = fcmp olt double %force_roundfr_rounded1155, %force_roundfr_rounded1152
  %mul_lo_cd_test1163 = fcmp olt double %force_roundfr_rounded1161, %force_roundfr_rounded1158
  %mul_lo_cd_min1164 = select i1 %mul_lo_cd_test1163, double %force_roundfr_rounded1161, double %force_roundfr_rounded1158
  %mul_lo_ab_min1165 = select i1 %mul_lo_ab_test1162, double %force_roundfr_rounded1155, double %force_roundfr_rounded1152
  %mul_lo_abcd_test1166 = fcmp olt double %mul_lo_cd_min1164, %mul_lo_ab_min1165
  %mul_lo_abcd_min1167 = select i1 %mul_lo_abcd_test1166, double %mul_lo_cd_min1164, double %mul_lo_ab_min1165
  call void @set_rounding_mode_up()
  %mul_hi_a1172 = fmul double %mul_hi_abcd_max1149, 6.700000e+01
  %force_round_fr_mem1173 = alloca double, align 8
  %302 = bitcast double* %force_round_fr_mem1173 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %302)
  store volatile double %mul_hi_a1172, double* %force_round_fr_mem1173, align 8
  %force_roundfr_rounded1174 = load volatile double* %force_round_fr_mem1173, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %302)
  %force_round_fr_mem1176 = alloca double, align 8
  %303 = bitcast double* %force_round_fr_mem1176 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %303)
  store volatile double %mul_hi_a1172, double* %force_round_fr_mem1176, align 8
  %force_roundfr_rounded1177 = load volatile double* %force_round_fr_mem1176, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %303)
  %mul_hi_c1178 = fmul double %mul_lo_abcd_min1167, 6.700000e+01
  %force_round_fr_mem1179 = alloca double, align 8
  %304 = bitcast double* %force_round_fr_mem1179 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %304)
  store volatile double %mul_hi_c1178, double* %force_round_fr_mem1179, align 8
  %force_roundfr_rounded1180 = load volatile double* %force_round_fr_mem1179, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %304)
  %force_round_fr_mem1182 = alloca double, align 8
  %305 = bitcast double* %force_round_fr_mem1182 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %305)
  store volatile double %mul_hi_c1178, double* %force_round_fr_mem1182, align 8
  %force_roundfr_rounded1183 = load volatile double* %force_round_fr_mem1182, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %305)
  %mul_hi_ab_test1184 = fcmp olt double %force_roundfr_rounded1174, %force_roundfr_rounded1177
  %mul_hi_cd_test1185 = fcmp olt double %force_roundfr_rounded1180, %force_roundfr_rounded1183
  %mul_hi_ab_max1186 = select i1 %mul_hi_ab_test1184, double %force_roundfr_rounded1177, double %force_roundfr_rounded1174
  %mul_hi_cd_max1187 = select i1 %mul_hi_cd_test1185, double %force_roundfr_rounded1183, double %force_roundfr_rounded1180
  %mul_hi_abcd_test1188 = fcmp olt double %mul_hi_ab_max1186, %mul_hi_cd_max1187
  %mul_hi_abcd_max1189 = select i1 %mul_hi_abcd_test1188, double %mul_hi_cd_max1187, double %mul_hi_ab_max1186
  call void @set_rounding_mode_down()
  %force_round_fr_mem1191 = alloca double, align 8
  %306 = bitcast double* %force_round_fr_mem1191 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %306)
  store volatile double %mul_hi_a1172, double* %force_round_fr_mem1191, align 8
  %force_roundfr_rounded1192 = load volatile double* %force_round_fr_mem1191, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %306)
  %force_round_fr_mem1194 = alloca double, align 8
  %307 = bitcast double* %force_round_fr_mem1194 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %307)
  store volatile double %mul_hi_a1172, double* %force_round_fr_mem1194, align 8
  %force_roundfr_rounded1195 = load volatile double* %force_round_fr_mem1194, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %307)
  %force_round_fr_mem1197 = alloca double, align 8
  %308 = bitcast double* %force_round_fr_mem1197 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %308)
  store volatile double %mul_hi_c1178, double* %force_round_fr_mem1197, align 8
  %force_roundfr_rounded1198 = load volatile double* %force_round_fr_mem1197, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %308)
  %force_round_fr_mem1200 = alloca double, align 8
  %309 = bitcast double* %force_round_fr_mem1200 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %309)
  store volatile double %mul_hi_c1178, double* %force_round_fr_mem1200, align 8
  %force_roundfr_rounded1201 = load volatile double* %force_round_fr_mem1200, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %309)
  %mul_lo_ab_test1202 = fcmp olt double %force_roundfr_rounded1195, %force_roundfr_rounded1192
  %mul_lo_cd_test1203 = fcmp olt double %force_roundfr_rounded1201, %force_roundfr_rounded1198
  %mul_lo_cd_min1204 = select i1 %mul_lo_cd_test1203, double %force_roundfr_rounded1201, double %force_roundfr_rounded1198
  %mul_lo_ab_min1205 = select i1 %mul_lo_ab_test1202, double %force_roundfr_rounded1195, double %force_roundfr_rounded1192
  %mul_lo_abcd_test1206 = fcmp olt double %mul_lo_cd_min1204, %mul_lo_ab_min1205
  %mul_lo_abcd_min1207 = select i1 %mul_lo_abcd_test1206, double %mul_lo_cd_min1204, double %mul_lo_ab_min1205
  call void @set_rounding_mode_up()
  %mul_hi_a1208 = fmul double %mul_lo_abcd_min1207, 3.500000e+01
  %force_round_fr_mem1209 = alloca double, align 8
  %310 = bitcast double* %force_round_fr_mem1209 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %310)
  store volatile double %mul_hi_a1208, double* %force_round_fr_mem1209, align 8
  %force_roundfr_rounded1210 = load volatile double* %force_round_fr_mem1209, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %310)
  %force_round_fr_mem1212 = alloca double, align 8
  %311 = bitcast double* %force_round_fr_mem1212 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %311)
  store volatile double %mul_hi_a1208, double* %force_round_fr_mem1212, align 8
  %force_roundfr_rounded1213 = load volatile double* %force_round_fr_mem1212, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %311)
  %mul_hi_c1214 = fmul double %mul_hi_abcd_max1189, 3.500000e+01
  %force_round_fr_mem1215 = alloca double, align 8
  %312 = bitcast double* %force_round_fr_mem1215 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %312)
  store volatile double %mul_hi_c1214, double* %force_round_fr_mem1215, align 8
  %force_roundfr_rounded1216 = load volatile double* %force_round_fr_mem1215, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %312)
  %force_round_fr_mem1218 = alloca double, align 8
  %313 = bitcast double* %force_round_fr_mem1218 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %313)
  store volatile double %mul_hi_c1214, double* %force_round_fr_mem1218, align 8
  %force_roundfr_rounded1219 = load volatile double* %force_round_fr_mem1218, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %313)
  %mul_hi_ab_test1220 = fcmp olt double %force_roundfr_rounded1210, %force_roundfr_rounded1213
  %mul_hi_cd_test1221 = fcmp olt double %force_roundfr_rounded1216, %force_roundfr_rounded1219
  %mul_hi_ab_max1222 = select i1 %mul_hi_ab_test1220, double %force_roundfr_rounded1213, double %force_roundfr_rounded1210
  %mul_hi_cd_max1223 = select i1 %mul_hi_cd_test1221, double %force_roundfr_rounded1219, double %force_roundfr_rounded1216
  %mul_hi_abcd_test1224 = fcmp olt double %mul_hi_ab_max1222, %mul_hi_cd_max1223
  %mul_hi_abcd_max1225 = select i1 %mul_hi_abcd_test1224, double %mul_hi_cd_max1223, double %mul_hi_ab_max1222
  call void @set_rounding_mode_down()
  %force_round_fr_mem1227 = alloca double, align 8
  %314 = bitcast double* %force_round_fr_mem1227 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %314)
  store volatile double %mul_hi_a1208, double* %force_round_fr_mem1227, align 8
  %force_roundfr_rounded1228 = load volatile double* %force_round_fr_mem1227, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %314)
  %force_round_fr_mem1230 = alloca double, align 8
  %315 = bitcast double* %force_round_fr_mem1230 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %315)
  store volatile double %mul_hi_a1208, double* %force_round_fr_mem1230, align 8
  %force_roundfr_rounded1231 = load volatile double* %force_round_fr_mem1230, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %315)
  %force_round_fr_mem1233 = alloca double, align 8
  %316 = bitcast double* %force_round_fr_mem1233 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %316)
  store volatile double %mul_hi_c1214, double* %force_round_fr_mem1233, align 8
  %force_roundfr_rounded1234 = load volatile double* %force_round_fr_mem1233, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %316)
  %force_round_fr_mem1236 = alloca double, align 8
  %317 = bitcast double* %force_round_fr_mem1236 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %317)
  store volatile double %mul_hi_c1214, double* %force_round_fr_mem1236, align 8
  %force_roundfr_rounded1237 = load volatile double* %force_round_fr_mem1236, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %317)
  %mul_lo_ab_test1238 = fcmp olt double %force_roundfr_rounded1231, %force_roundfr_rounded1228
  %mul_lo_cd_test1239 = fcmp olt double %force_roundfr_rounded1237, %force_roundfr_rounded1234
  %mul_lo_cd_min1240 = select i1 %mul_lo_cd_test1239, double %force_roundfr_rounded1237, double %force_roundfr_rounded1234
  %mul_lo_ab_min1241 = select i1 %mul_lo_ab_test1238, double %force_roundfr_rounded1231, double %force_roundfr_rounded1228
  %mul_lo_abcd_test1242 = fcmp olt double %mul_lo_cd_min1240, %mul_lo_ab_min1241
  %mul_lo_abcd_min1243 = select i1 %mul_lo_abcd_test1242, double %mul_lo_cd_min1240, double %mul_lo_ab_min1241
  call void @set_rounding_mode_up()
  %mul_hi_a1244 = fmul double %mul_lo_abcd_min1243, %o_lo
  %force_round_fr_mem1245 = alloca double, align 8
  %318 = bitcast double* %force_round_fr_mem1245 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %318)
  store volatile double %mul_hi_a1244, double* %force_round_fr_mem1245, align 8
  %force_roundfr_rounded1246 = load volatile double* %force_round_fr_mem1245, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %318)
  %mul_hi_b1247 = fmul double %mul_hi_abcd_max1225, %o_lo
  %force_round_fr_mem1248 = alloca double, align 8
  %319 = bitcast double* %force_round_fr_mem1248 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %319)
  store volatile double %mul_hi_b1247, double* %force_round_fr_mem1248, align 8
  %force_roundfr_rounded1249 = load volatile double* %force_round_fr_mem1248, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %319)
  %mul_hi_c1250 = fmul double %mul_lo_abcd_min1243, %o_hi
  %force_round_fr_mem1251 = alloca double, align 8
  %320 = bitcast double* %force_round_fr_mem1251 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %320)
  store volatile double %mul_hi_c1250, double* %force_round_fr_mem1251, align 8
  %force_roundfr_rounded1252 = load volatile double* %force_round_fr_mem1251, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %320)
  %mul_hi_d1253 = fmul double %mul_hi_abcd_max1225, %o_hi
  %force_round_fr_mem1254 = alloca double, align 8
  %321 = bitcast double* %force_round_fr_mem1254 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %321)
  store volatile double %mul_hi_d1253, double* %force_round_fr_mem1254, align 8
  %force_roundfr_rounded1255 = load volatile double* %force_round_fr_mem1254, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %321)
  %mul_hi_ab_test1256 = fcmp olt double %force_roundfr_rounded1246, %force_roundfr_rounded1249
  %mul_hi_cd_test1257 = fcmp olt double %force_roundfr_rounded1252, %force_roundfr_rounded1255
  %mul_hi_ab_max1258 = select i1 %mul_hi_ab_test1256, double %force_roundfr_rounded1249, double %force_roundfr_rounded1246
  %mul_hi_cd_max1259 = select i1 %mul_hi_cd_test1257, double %force_roundfr_rounded1255, double %force_roundfr_rounded1252
  %mul_hi_abcd_test1260 = fcmp olt double %mul_hi_ab_max1258, %mul_hi_cd_max1259
  %mul_hi_abcd_max1261 = select i1 %mul_hi_abcd_test1260, double %mul_hi_cd_max1259, double %mul_hi_ab_max1258
  call void @set_rounding_mode_down()
  %force_round_fr_mem1263 = alloca double, align 8
  %322 = bitcast double* %force_round_fr_mem1263 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %322)
  store volatile double %mul_hi_a1244, double* %force_round_fr_mem1263, align 8
  %force_roundfr_rounded1264 = load volatile double* %force_round_fr_mem1263, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %322)
  %force_round_fr_mem1266 = alloca double, align 8
  %323 = bitcast double* %force_round_fr_mem1266 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %323)
  store volatile double %mul_hi_b1247, double* %force_round_fr_mem1266, align 8
  %force_roundfr_rounded1267 = load volatile double* %force_round_fr_mem1266, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %323)
  %force_round_fr_mem1269 = alloca double, align 8
  %324 = bitcast double* %force_round_fr_mem1269 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %324)
  store volatile double %mul_hi_c1250, double* %force_round_fr_mem1269, align 8
  %force_roundfr_rounded1270 = load volatile double* %force_round_fr_mem1269, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %324)
  %force_round_fr_mem1272 = alloca double, align 8
  %325 = bitcast double* %force_round_fr_mem1272 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %325)
  store volatile double %mul_hi_d1253, double* %force_round_fr_mem1272, align 8
  %force_roundfr_rounded1273 = load volatile double* %force_round_fr_mem1272, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %325)
  %mul_lo_ab_test1274 = fcmp olt double %force_roundfr_rounded1267, %force_roundfr_rounded1264
  %mul_lo_cd_test1275 = fcmp olt double %force_roundfr_rounded1273, %force_roundfr_rounded1270
  %mul_lo_cd_min1276 = select i1 %mul_lo_cd_test1275, double %force_roundfr_rounded1273, double %force_roundfr_rounded1270
  %mul_lo_ab_min1277 = select i1 %mul_lo_ab_test1274, double %force_roundfr_rounded1267, double %force_roundfr_rounded1264
  %mul_lo_abcd_test1278 = fcmp olt double %mul_lo_cd_min1276, %mul_lo_ab_min1277
  %mul_lo_abcd_min1279 = select i1 %mul_lo_abcd_test1278, double %mul_lo_cd_min1276, double %mul_lo_ab_min1277
  %expt_even_are_both_pos_p1282 = fcmp olt double %mul_hi_abcd_max1261, -0.000000e+00
  call void @set_rounding_mode_up()
  br i1 %expt_even_are_both_pos_p1282, label %expt_even_both_are_pos1283, label %expt_even_both_are_not_pos1302

expt_even_both_are_pos1283:                       ; preds = %expt_even_merge1093
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x1284 = fmul double %mul_hi_abcd_max1261, %mul_hi_abcd_max1261
  %force_round_fr_mem1285 = alloca double, align 8
  %326 = bitcast double* %force_round_fr_mem1285 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %326)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1284, double* %force_round_fr_mem1285, align 8
  %force_roundfr_rounded1286 = load volatile double* %force_round_fr_mem1285, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %326)
  %expt_even_pos_block_res_lo_unroll_x1287 = fmul double %force_roundfr_rounded1286, %force_roundfr_rounded1286
  %force_round_fr_mem1288 = alloca double, align 8
  %327 = bitcast double* %force_round_fr_mem1288 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %327)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1287, double* %force_round_fr_mem1288, align 8
  %force_roundfr_rounded1289 = load volatile double* %force_round_fr_mem1288, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %327)
  %force_round_fr_mem1291 = alloca double, align 8
  %328 = bitcast double* %force_round_fr_mem1291 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %328)
  store volatile double %force_roundfr_rounded1289, double* %force_round_fr_mem1291, align 8
  %force_roundfr_rounded1292 = load volatile double* %force_round_fr_mem1291, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %328)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1293 = fmul double %mul_lo_abcd_min1279, %mul_lo_abcd_min1279
  %force_round_fr_mem1294 = alloca double, align 8
  %329 = bitcast double* %force_round_fr_mem1294 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %329)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1293, double* %force_round_fr_mem1294, align 8
  %force_roundfr_rounded1295 = load volatile double* %force_round_fr_mem1294, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %329)
  %expt_even_pos_block_res_hi_unroll_x1296 = fmul double %force_roundfr_rounded1295, %force_roundfr_rounded1295
  %force_round_fr_mem1297 = alloca double, align 8
  %330 = bitcast double* %force_round_fr_mem1297 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %330)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1296, double* %force_round_fr_mem1297, align 8
  %force_roundfr_rounded1298 = load volatile double* %force_round_fr_mem1297, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %330)
  %force_round_fr_mem1300 = alloca double, align 8
  %331 = bitcast double* %force_round_fr_mem1300 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %331)
  store volatile double %force_roundfr_rounded1298, double* %force_round_fr_mem1300, align 8
  %force_roundfr_rounded1301 = load volatile double* %force_round_fr_mem1300, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %331)
  br label %expt_even_merge1344

expt_even_both_are_not_pos1302:                   ; preds = %expt_even_merge1093
  %expt_even_are_both_neg_p1303 = fcmp ogt double %mul_lo_abcd_min1279, -0.000000e+00
  br i1 %expt_even_are_both_neg_p1303, label %expt_even_both_are_neg1304, label %expt_even_contains_zero1323

expt_even_both_are_neg1304:                       ; preds = %expt_even_both_are_not_pos1302
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x1305 = fmul double %mul_lo_abcd_min1279, %mul_lo_abcd_min1279
  %force_round_fr_mem1306 = alloca double, align 8
  %332 = bitcast double* %force_round_fr_mem1306 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %332)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1305, double* %force_round_fr_mem1306, align 8
  %force_roundfr_rounded1307 = load volatile double* %force_round_fr_mem1306, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %332)
  %expt_even_neg_block_res_lo_unroll_x1308 = fmul double %force_roundfr_rounded1307, %force_roundfr_rounded1307
  %force_round_fr_mem1309 = alloca double, align 8
  %333 = bitcast double* %force_round_fr_mem1309 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %333)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1308, double* %force_round_fr_mem1309, align 8
  %force_roundfr_rounded1310 = load volatile double* %force_round_fr_mem1309, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %333)
  %force_round_fr_mem1312 = alloca double, align 8
  %334 = bitcast double* %force_round_fr_mem1312 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %334)
  store volatile double %force_roundfr_rounded1310, double* %force_round_fr_mem1312, align 8
  %force_roundfr_rounded1313 = load volatile double* %force_round_fr_mem1312, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %334)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1314 = fmul double %mul_hi_abcd_max1261, %mul_hi_abcd_max1261
  %force_round_fr_mem1315 = alloca double, align 8
  %335 = bitcast double* %force_round_fr_mem1315 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %335)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1314, double* %force_round_fr_mem1315, align 8
  %force_roundfr_rounded1316 = load volatile double* %force_round_fr_mem1315, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %335)
  %expt_even_pos_block_res_hi_unroll_x1317 = fmul double %force_roundfr_rounded1316, %force_roundfr_rounded1316
  %force_round_fr_mem1318 = alloca double, align 8
  %336 = bitcast double* %force_round_fr_mem1318 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %336)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1317, double* %force_round_fr_mem1318, align 8
  %force_roundfr_rounded1319 = load volatile double* %force_round_fr_mem1318, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %336)
  %force_round_fr_mem1321 = alloca double, align 8
  %337 = bitcast double* %force_round_fr_mem1321 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %337)
  store volatile double %force_roundfr_rounded1319, double* %force_round_fr_mem1321, align 8
  %force_roundfr_rounded1322 = load volatile double* %force_round_fr_mem1321, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %337)
  br label %expt_even_merge1344

expt_even_contains_zero1323:                      ; preds = %expt_even_both_are_not_pos1302
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x1324 = fmul double %mul_hi_abcd_max1261, %mul_hi_abcd_max1261
  %force_round_fr_mem1325 = alloca double, align 8
  %338 = bitcast double* %force_round_fr_mem1325 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %338)
  store volatile double %expt_even_mid_block_lo__unroll_x1324, double* %force_round_fr_mem1325, align 8
  %force_roundfr_rounded1326 = load volatile double* %force_round_fr_mem1325, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %338)
  %expt_even_mid_block_lo__unroll_x1327 = fmul double %force_roundfr_rounded1326, %force_roundfr_rounded1326
  %force_round_fr_mem1328 = alloca double, align 8
  %339 = bitcast double* %force_round_fr_mem1328 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %339)
  store volatile double %expt_even_mid_block_lo__unroll_x1327, double* %force_round_fr_mem1328, align 8
  %force_roundfr_rounded1329 = load volatile double* %force_round_fr_mem1328, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %339)
  %force_round_fr_mem1331 = alloca double, align 8
  %340 = bitcast double* %force_round_fr_mem1331 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %340)
  store volatile double %force_roundfr_rounded1329, double* %force_round_fr_mem1331, align 8
  %force_roundfr_rounded1332 = load volatile double* %force_round_fr_mem1331, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %340)
  %expt_even_mid_block_hi__unroll_x1333 = fmul double %mul_lo_abcd_min1279, %mul_lo_abcd_min1279
  %force_round_fr_mem1334 = alloca double, align 8
  %341 = bitcast double* %force_round_fr_mem1334 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %341)
  store volatile double %expt_even_mid_block_hi__unroll_x1333, double* %force_round_fr_mem1334, align 8
  %force_roundfr_rounded1335 = load volatile double* %force_round_fr_mem1334, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %341)
  %expt_even_mid_block_hi__unroll_x1336 = fmul double %force_roundfr_rounded1335, %force_roundfr_rounded1335
  %force_round_fr_mem1337 = alloca double, align 8
  %342 = bitcast double* %force_round_fr_mem1337 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %342)
  store volatile double %expt_even_mid_block_hi__unroll_x1336, double* %force_round_fr_mem1337, align 8
  %force_roundfr_rounded1338 = load volatile double* %force_round_fr_mem1337, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %342)
  %force_round_fr_mem1340 = alloca double, align 8
  %343 = bitcast double* %force_round_fr_mem1340 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %343)
  store volatile double %force_roundfr_rounded1338, double* %force_round_fr_mem1340, align 8
  %force_roundfr_rounded1341 = load volatile double* %force_round_fr_mem1340, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %343)
  %expt_even_mid_block_res_hi_abtest1342 = fcmp olt double %force_roundfr_rounded1332, %force_roundfr_rounded1341
  %expt_even_mid_block_res_hi_abmax1343 = select i1 %expt_even_mid_block_res_hi_abtest1342, double %force_roundfr_rounded1341, double %force_roundfr_rounded1332
  br label %expt_even_merge1344

expt_even_merge1344:                              ; preds = %expt_even_contains_zero1323, %expt_even_both_are_neg1304, %expt_even_both_are_pos1283
  %expt_lo1345 = phi double [ %force_roundfr_rounded1292, %expt_even_both_are_pos1283 ], [ %force_roundfr_rounded1313, %expt_even_both_are_neg1304 ], [ 0.000000e+00, %expt_even_contains_zero1323 ]
  %expt_hi1346 = phi double [ %force_roundfr_rounded1301, %expt_even_both_are_pos1283 ], [ %force_roundfr_rounded1322, %expt_even_both_are_neg1304 ], [ %expt_even_mid_block_res_hi_abmax1343, %expt_even_contains_zero1323 ]
  %div_hi_a1347 = fdiv double -8.500000e+01, %expt_lo1345
  %force_round_fr_mem1348 = alloca double, align 8
  %344 = bitcast double* %force_round_fr_mem1348 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %344)
  store volatile double %div_hi_a1347, double* %force_round_fr_mem1348, align 8
  %force_roundfr_rounded1349 = load volatile double* %force_round_fr_mem1348, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %344)
  %div_hi_b1350 = fdiv double -8.500000e+01, %expt_hi1346
  %force_round_fr_mem1351 = alloca double, align 8
  %345 = bitcast double* %force_round_fr_mem1351 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %345)
  store volatile double %div_hi_b1350, double* %force_round_fr_mem1351, align 8
  %force_roundfr_rounded1352 = load volatile double* %force_round_fr_mem1351, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %345)
  %force_round_fr_mem1354 = alloca double, align 8
  %346 = bitcast double* %force_round_fr_mem1354 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %346)
  store volatile double %div_hi_a1347, double* %force_round_fr_mem1354, align 8
  %force_roundfr_rounded1355 = load volatile double* %force_round_fr_mem1354, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %346)
  %force_round_fr_mem1357 = alloca double, align 8
  %347 = bitcast double* %force_round_fr_mem1357 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %347)
  store volatile double %div_hi_b1350, double* %force_round_fr_mem1357, align 8
  %force_roundfr_rounded1358 = load volatile double* %force_round_fr_mem1357, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %347)
  %div_hi_ab_test1359 = fcmp olt double %force_roundfr_rounded1349, %force_roundfr_rounded1352
  %div_hi_cd_test1360 = fcmp olt double %force_roundfr_rounded1355, %force_roundfr_rounded1358
  %div_hi_ab_max1361 = select i1 %div_hi_ab_test1359, double %force_roundfr_rounded1352, double %force_roundfr_rounded1349
  %div_hi_cd_max1362 = select i1 %div_hi_cd_test1360, double %force_roundfr_rounded1358, double %force_roundfr_rounded1355
  %div_hi_abcd_test1363 = fcmp olt double %div_hi_ab_max1361, %div_hi_cd_max1362
  %div_hi_abcd_max1364 = select i1 %div_hi_abcd_test1363, double %div_hi_cd_max1362, double %div_hi_ab_max1361
  call void @set_rounding_mode_down()
  %force_round_fr_mem1366 = alloca double, align 8
  %348 = bitcast double* %force_round_fr_mem1366 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %348)
  store volatile double %div_hi_a1347, double* %force_round_fr_mem1366, align 8
  %force_roundfr_rounded1367 = load volatile double* %force_round_fr_mem1366, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %348)
  %force_round_fr_mem1369 = alloca double, align 8
  %349 = bitcast double* %force_round_fr_mem1369 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %349)
  store volatile double %div_hi_b1350, double* %force_round_fr_mem1369, align 8
  %force_roundfr_rounded1370 = load volatile double* %force_round_fr_mem1369, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %349)
  %force_round_fr_mem1372 = alloca double, align 8
  %350 = bitcast double* %force_round_fr_mem1372 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %350)
  store volatile double %div_hi_a1347, double* %force_round_fr_mem1372, align 8
  %force_roundfr_rounded1373 = load volatile double* %force_round_fr_mem1372, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %350)
  %force_round_fr_mem1375 = alloca double, align 8
  %351 = bitcast double* %force_round_fr_mem1375 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %351)
  store volatile double %div_hi_b1350, double* %force_round_fr_mem1375, align 8
  %force_roundfr_rounded1376 = load volatile double* %force_round_fr_mem1375, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %351)
  %div_lo_ab_test1377 = fcmp olt double %force_roundfr_rounded1370, %force_roundfr_rounded1367
  %div_lo_cd_test1378 = fcmp olt double %force_roundfr_rounded1376, %force_roundfr_rounded1373
  %div_lo_cd_min1379 = select i1 %div_lo_cd_test1378, double %force_roundfr_rounded1376, double %force_roundfr_rounded1373
  %div_lo_ab_min1380 = select i1 %div_lo_ab_test1377, double %force_roundfr_rounded1370, double %force_roundfr_rounded1367
  %div_lo_abcd_test1381 = fcmp olt double %div_lo_cd_min1379, %div_lo_ab_min1380
  %div_lo_abcd_min1382 = select i1 %div_lo_abcd_test1381, double %div_lo_cd_min1379, double %div_lo_ab_min1380
  %expt_odd_lo_unroll_x1383 = fmul double %div_lo_abcd_min1382, %div_lo_abcd_min1382
  %force_round_fr_mem1384 = alloca double, align 8
  %352 = bitcast double* %force_round_fr_mem1384 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %352)
  store volatile double %expt_odd_lo_unroll_x1383, double* %force_round_fr_mem1384, align 8
  %force_roundfr_rounded1385 = load volatile double* %force_round_fr_mem1384, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %352)
  %expt_odd_lopwr_do_y1386 = fmul double %force_roundfr_rounded1385, %div_lo_abcd_min1382
  %force_round_fr_mem1387 = alloca double, align 8
  %353 = bitcast double* %force_round_fr_mem1387 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %353)
  store volatile double %expt_odd_lopwr_do_y1386, double* %force_round_fr_mem1387, align 8
  %force_roundfr_rounded1388 = load volatile double* %force_round_fr_mem1387, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %353)
  call void @set_rounding_mode_up()
  %expt_odd_hi_unroll_x1389 = fmul double %div_hi_abcd_max1364, %div_hi_abcd_max1364
  %force_round_fr_mem1390 = alloca double, align 8
  %354 = bitcast double* %force_round_fr_mem1390 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %354)
  store volatile double %expt_odd_hi_unroll_x1389, double* %force_round_fr_mem1390, align 8
  %force_roundfr_rounded1391 = load volatile double* %force_round_fr_mem1390, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %354)
  %expt_odd_hipwr_do_y1392 = fmul double %force_roundfr_rounded1391, %div_hi_abcd_max1364
  %force_round_fr_mem1393 = alloca double, align 8
  %355 = bitcast double* %force_round_fr_mem1393 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %355)
  store volatile double %expt_odd_hipwr_do_y1392, double* %force_round_fr_mem1393, align 8
  %force_roundfr_rounded1394 = load volatile double* %force_round_fr_mem1393, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %355)
  %div_hi_a1395 = fdiv double %p_lo, %force_roundfr_rounded1388
  %force_round_fr_mem1396 = alloca double, align 8
  %356 = bitcast double* %force_round_fr_mem1396 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %356)
  store volatile double %div_hi_a1395, double* %force_round_fr_mem1396, align 8
  %force_roundfr_rounded1397 = load volatile double* %force_round_fr_mem1396, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %356)
  %div_hi_b1398 = fdiv double %p_lo, %force_roundfr_rounded1394
  %force_round_fr_mem1399 = alloca double, align 8
  %357 = bitcast double* %force_round_fr_mem1399 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %357)
  store volatile double %div_hi_b1398, double* %force_round_fr_mem1399, align 8
  %force_roundfr_rounded1400 = load volatile double* %force_round_fr_mem1399, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %357)
  %div_hi_c1401 = fdiv double %p_hi, %force_roundfr_rounded1388
  %force_round_fr_mem1402 = alloca double, align 8
  %358 = bitcast double* %force_round_fr_mem1402 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %358)
  store volatile double %div_hi_c1401, double* %force_round_fr_mem1402, align 8
  %force_roundfr_rounded1403 = load volatile double* %force_round_fr_mem1402, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %358)
  %div_hi_d1404 = fdiv double %p_hi, %force_roundfr_rounded1394
  %force_round_fr_mem1405 = alloca double, align 8
  %359 = bitcast double* %force_round_fr_mem1405 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %359)
  store volatile double %div_hi_d1404, double* %force_round_fr_mem1405, align 8
  %force_roundfr_rounded1406 = load volatile double* %force_round_fr_mem1405, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %359)
  %div_hi_ab_test1407 = fcmp olt double %force_roundfr_rounded1397, %force_roundfr_rounded1400
  %div_hi_cd_test1408 = fcmp olt double %force_roundfr_rounded1403, %force_roundfr_rounded1406
  %div_hi_ab_max1409 = select i1 %div_hi_ab_test1407, double %force_roundfr_rounded1400, double %force_roundfr_rounded1397
  %div_hi_cd_max1410 = select i1 %div_hi_cd_test1408, double %force_roundfr_rounded1406, double %force_roundfr_rounded1403
  %div_hi_abcd_test1411 = fcmp olt double %div_hi_ab_max1409, %div_hi_cd_max1410
  %div_hi_abcd_max1412 = select i1 %div_hi_abcd_test1411, double %div_hi_cd_max1410, double %div_hi_ab_max1409
  call void @set_rounding_mode_down()
  %force_round_fr_mem1414 = alloca double, align 8
  %360 = bitcast double* %force_round_fr_mem1414 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %360)
  store volatile double %div_hi_a1395, double* %force_round_fr_mem1414, align 8
  %force_roundfr_rounded1415 = load volatile double* %force_round_fr_mem1414, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %360)
  %force_round_fr_mem1417 = alloca double, align 8
  %361 = bitcast double* %force_round_fr_mem1417 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %361)
  store volatile double %div_hi_b1398, double* %force_round_fr_mem1417, align 8
  %force_roundfr_rounded1418 = load volatile double* %force_round_fr_mem1417, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %361)
  %force_round_fr_mem1420 = alloca double, align 8
  %362 = bitcast double* %force_round_fr_mem1420 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %362)
  store volatile double %div_hi_c1401, double* %force_round_fr_mem1420, align 8
  %force_roundfr_rounded1421 = load volatile double* %force_round_fr_mem1420, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %362)
  %force_round_fr_mem1423 = alloca double, align 8
  %363 = bitcast double* %force_round_fr_mem1423 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %363)
  store volatile double %div_hi_d1404, double* %force_round_fr_mem1423, align 8
  %force_roundfr_rounded1424 = load volatile double* %force_round_fr_mem1423, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %363)
  %div_lo_ab_test1425 = fcmp olt double %force_roundfr_rounded1418, %force_roundfr_rounded1415
  %div_lo_cd_test1426 = fcmp olt double %force_roundfr_rounded1424, %force_roundfr_rounded1421
  %div_lo_cd_min1427 = select i1 %div_lo_cd_test1426, double %force_roundfr_rounded1424, double %force_roundfr_rounded1421
  %div_lo_ab_min1428 = select i1 %div_lo_ab_test1425, double %force_roundfr_rounded1418, double %force_roundfr_rounded1415
  %div_lo_abcd_test1429 = fcmp olt double %div_lo_cd_min1427, %div_lo_ab_min1428
  %div_lo_abcd_min1430 = select i1 %div_lo_abcd_test1429, double %div_lo_cd_min1427, double %div_lo_ab_min1428
  call void @set_rounding_mode_up()
  %mul_hi_a1433 = fmul double %div_lo_abcd_min1430, -1.900000e+01
  %force_round_fr_mem1434 = alloca double, align 8
  %364 = bitcast double* %force_round_fr_mem1434 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %364)
  store volatile double %mul_hi_a1433, double* %force_round_fr_mem1434, align 8
  %force_roundfr_rounded1435 = load volatile double* %force_round_fr_mem1434, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %364)
  %force_round_fr_mem1437 = alloca double, align 8
  %365 = bitcast double* %force_round_fr_mem1437 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %365)
  store volatile double %mul_hi_a1433, double* %force_round_fr_mem1437, align 8
  %force_roundfr_rounded1438 = load volatile double* %force_round_fr_mem1437, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %365)
  %mul_hi_c1439 = fmul double %div_hi_abcd_max1412, -1.900000e+01
  %force_round_fr_mem1440 = alloca double, align 8
  %366 = bitcast double* %force_round_fr_mem1440 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %366)
  store volatile double %mul_hi_c1439, double* %force_round_fr_mem1440, align 8
  %force_roundfr_rounded1441 = load volatile double* %force_round_fr_mem1440, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %366)
  %force_round_fr_mem1443 = alloca double, align 8
  %367 = bitcast double* %force_round_fr_mem1443 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %367)
  store volatile double %mul_hi_c1439, double* %force_round_fr_mem1443, align 8
  %force_roundfr_rounded1444 = load volatile double* %force_round_fr_mem1443, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %367)
  %mul_hi_ab_test1445 = fcmp olt double %force_roundfr_rounded1435, %force_roundfr_rounded1438
  %mul_hi_cd_test1446 = fcmp olt double %force_roundfr_rounded1441, %force_roundfr_rounded1444
  %mul_hi_ab_max1447 = select i1 %mul_hi_ab_test1445, double %force_roundfr_rounded1438, double %force_roundfr_rounded1435
  %mul_hi_cd_max1448 = select i1 %mul_hi_cd_test1446, double %force_roundfr_rounded1444, double %force_roundfr_rounded1441
  %mul_hi_abcd_test1449 = fcmp olt double %mul_hi_ab_max1447, %mul_hi_cd_max1448
  %mul_hi_abcd_max1450 = select i1 %mul_hi_abcd_test1449, double %mul_hi_cd_max1448, double %mul_hi_ab_max1447
  call void @set_rounding_mode_down()
  %force_round_fr_mem1452 = alloca double, align 8
  %368 = bitcast double* %force_round_fr_mem1452 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %368)
  store volatile double %mul_hi_a1433, double* %force_round_fr_mem1452, align 8
  %force_roundfr_rounded1453 = load volatile double* %force_round_fr_mem1452, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %368)
  %force_round_fr_mem1455 = alloca double, align 8
  %369 = bitcast double* %force_round_fr_mem1455 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %369)
  store volatile double %mul_hi_a1433, double* %force_round_fr_mem1455, align 8
  %force_roundfr_rounded1456 = load volatile double* %force_round_fr_mem1455, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %369)
  %force_round_fr_mem1458 = alloca double, align 8
  %370 = bitcast double* %force_round_fr_mem1458 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %370)
  store volatile double %mul_hi_c1439, double* %force_round_fr_mem1458, align 8
  %force_roundfr_rounded1459 = load volatile double* %force_round_fr_mem1458, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %370)
  %force_round_fr_mem1461 = alloca double, align 8
  %371 = bitcast double* %force_round_fr_mem1461 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %371)
  store volatile double %mul_hi_c1439, double* %force_round_fr_mem1461, align 8
  %force_roundfr_rounded1462 = load volatile double* %force_round_fr_mem1461, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %371)
  %mul_lo_ab_test1463 = fcmp olt double %force_roundfr_rounded1456, %force_roundfr_rounded1453
  %mul_lo_cd_test1464 = fcmp olt double %force_roundfr_rounded1462, %force_roundfr_rounded1459
  %mul_lo_cd_min1465 = select i1 %mul_lo_cd_test1464, double %force_roundfr_rounded1462, double %force_roundfr_rounded1459
  %mul_lo_ab_min1466 = select i1 %mul_lo_ab_test1463, double %force_roundfr_rounded1456, double %force_roundfr_rounded1453
  %mul_lo_abcd_test1467 = fcmp olt double %mul_lo_cd_min1465, %mul_lo_ab_min1466
  %mul_lo_abcd_min1468 = select i1 %mul_lo_abcd_test1467, double %mul_lo_cd_min1465, double %mul_lo_ab_min1466
  %expt_even_are_both_pos_p1469 = fcmp ogt double %mul_lo_abcd_min1468, 0.000000e+00
  call void @set_rounding_mode_up()
  br i1 %expt_even_are_both_pos_p1469, label %expt_even_both_are_pos1470, label %expt_even_both_are_not_pos1483

expt_even_both_are_pos1470:                       ; preds = %expt_even_merge1344
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x1471 = fmul double %mul_lo_abcd_min1468, %mul_lo_abcd_min1468
  %force_round_fr_mem1472 = alloca double, align 8
  %372 = bitcast double* %force_round_fr_mem1472 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %372)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1471, double* %force_round_fr_mem1472, align 8
  %force_roundfr_rounded1473 = load volatile double* %force_round_fr_mem1472, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %372)
  %force_round_fr_mem1475 = alloca double, align 8
  %373 = bitcast double* %force_round_fr_mem1475 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %373)
  store volatile double %force_roundfr_rounded1473, double* %force_round_fr_mem1475, align 8
  %force_roundfr_rounded1476 = load volatile double* %force_round_fr_mem1475, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %373)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1477 = fmul double %mul_hi_abcd_max1450, %mul_hi_abcd_max1450
  %force_round_fr_mem1478 = alloca double, align 8
  %374 = bitcast double* %force_round_fr_mem1478 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %374)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1477, double* %force_round_fr_mem1478, align 8
  %force_roundfr_rounded1479 = load volatile double* %force_round_fr_mem1478, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %374)
  %force_round_fr_mem1481 = alloca double, align 8
  %375 = bitcast double* %force_round_fr_mem1481 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %375)
  store volatile double %force_roundfr_rounded1479, double* %force_round_fr_mem1481, align 8
  %force_roundfr_rounded1482 = load volatile double* %force_round_fr_mem1481, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %375)
  br label %expt_even_merge1513

expt_even_both_are_not_pos1483:                   ; preds = %expt_even_merge1344
  %expt_even_are_both_neg_p1484 = fcmp olt double %mul_hi_abcd_max1450, 0.000000e+00
  br i1 %expt_even_are_both_neg_p1484, label %expt_even_both_are_neg1485, label %expt_even_contains_zero1498

expt_even_both_are_neg1485:                       ; preds = %expt_even_both_are_not_pos1483
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x1486 = fmul double %mul_hi_abcd_max1450, %mul_hi_abcd_max1450
  %force_round_fr_mem1487 = alloca double, align 8
  %376 = bitcast double* %force_round_fr_mem1487 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %376)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1486, double* %force_round_fr_mem1487, align 8
  %force_roundfr_rounded1488 = load volatile double* %force_round_fr_mem1487, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %376)
  %force_round_fr_mem1490 = alloca double, align 8
  %377 = bitcast double* %force_round_fr_mem1490 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %377)
  store volatile double %force_roundfr_rounded1488, double* %force_round_fr_mem1490, align 8
  %force_roundfr_rounded1491 = load volatile double* %force_round_fr_mem1490, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %377)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1492 = fmul double %mul_lo_abcd_min1468, %mul_lo_abcd_min1468
  %force_round_fr_mem1493 = alloca double, align 8
  %378 = bitcast double* %force_round_fr_mem1493 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %378)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1492, double* %force_round_fr_mem1493, align 8
  %force_roundfr_rounded1494 = load volatile double* %force_round_fr_mem1493, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %378)
  %force_round_fr_mem1496 = alloca double, align 8
  %379 = bitcast double* %force_round_fr_mem1496 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %379)
  store volatile double %force_roundfr_rounded1494, double* %force_round_fr_mem1496, align 8
  %force_roundfr_rounded1497 = load volatile double* %force_round_fr_mem1496, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %379)
  br label %expt_even_merge1513

expt_even_contains_zero1498:                      ; preds = %expt_even_both_are_not_pos1483
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x1499 = fmul double %mul_lo_abcd_min1468, %mul_lo_abcd_min1468
  %force_round_fr_mem1500 = alloca double, align 8
  %380 = bitcast double* %force_round_fr_mem1500 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %380)
  store volatile double %expt_even_mid_block_lo__unroll_x1499, double* %force_round_fr_mem1500, align 8
  %force_roundfr_rounded1501 = load volatile double* %force_round_fr_mem1500, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %380)
  %force_round_fr_mem1503 = alloca double, align 8
  %381 = bitcast double* %force_round_fr_mem1503 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %381)
  store volatile double %force_roundfr_rounded1501, double* %force_round_fr_mem1503, align 8
  %force_roundfr_rounded1504 = load volatile double* %force_round_fr_mem1503, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %381)
  %expt_even_mid_block_hi__unroll_x1505 = fmul double %mul_hi_abcd_max1450, %mul_hi_abcd_max1450
  %force_round_fr_mem1506 = alloca double, align 8
  %382 = bitcast double* %force_round_fr_mem1506 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %382)
  store volatile double %expt_even_mid_block_hi__unroll_x1505, double* %force_round_fr_mem1506, align 8
  %force_roundfr_rounded1507 = load volatile double* %force_round_fr_mem1506, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %382)
  %force_round_fr_mem1509 = alloca double, align 8
  %383 = bitcast double* %force_round_fr_mem1509 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %383)
  store volatile double %force_roundfr_rounded1507, double* %force_round_fr_mem1509, align 8
  %force_roundfr_rounded1510 = load volatile double* %force_round_fr_mem1509, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %383)
  %expt_even_mid_block_res_hi_abtest1511 = fcmp olt double %force_roundfr_rounded1504, %force_roundfr_rounded1510
  %expt_even_mid_block_res_hi_abmax1512 = select i1 %expt_even_mid_block_res_hi_abtest1511, double %force_roundfr_rounded1510, double %force_roundfr_rounded1504
  br label %expt_even_merge1513

expt_even_merge1513:                              ; preds = %expt_even_contains_zero1498, %expt_even_both_are_neg1485, %expt_even_both_are_pos1470
  %expt_lo1514 = phi double [ %force_roundfr_rounded1476, %expt_even_both_are_pos1470 ], [ %force_roundfr_rounded1491, %expt_even_both_are_neg1485 ], [ 0.000000e+00, %expt_even_contains_zero1498 ]
  %expt_hi1515 = phi double [ %force_roundfr_rounded1482, %expt_even_both_are_pos1470 ], [ %force_roundfr_rounded1497, %expt_even_both_are_neg1485 ], [ %expt_even_mid_block_res_hi_abmax1512, %expt_even_contains_zero1498 ]
  %mul_hi_a1516 = fmul double %expt_lo1514, %q_lo
  %force_round_fr_mem1517 = alloca double, align 8
  %384 = bitcast double* %force_round_fr_mem1517 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %384)
  store volatile double %mul_hi_a1516, double* %force_round_fr_mem1517, align 8
  %force_roundfr_rounded1518 = load volatile double* %force_round_fr_mem1517, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %384)
  %mul_hi_b1519 = fmul double %expt_hi1515, %q_lo
  %force_round_fr_mem1520 = alloca double, align 8
  %385 = bitcast double* %force_round_fr_mem1520 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %385)
  store volatile double %mul_hi_b1519, double* %force_round_fr_mem1520, align 8
  %force_roundfr_rounded1521 = load volatile double* %force_round_fr_mem1520, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %385)
  %mul_hi_c1522 = fmul double %expt_lo1514, %q_hi
  %force_round_fr_mem1523 = alloca double, align 8
  %386 = bitcast double* %force_round_fr_mem1523 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %386)
  store volatile double %mul_hi_c1522, double* %force_round_fr_mem1523, align 8
  %force_roundfr_rounded1524 = load volatile double* %force_round_fr_mem1523, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %386)
  %mul_hi_d1525 = fmul double %expt_hi1515, %q_hi
  %force_round_fr_mem1526 = alloca double, align 8
  %387 = bitcast double* %force_round_fr_mem1526 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %387)
  store volatile double %mul_hi_d1525, double* %force_round_fr_mem1526, align 8
  %force_roundfr_rounded1527 = load volatile double* %force_round_fr_mem1526, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %387)
  %mul_hi_ab_test1528 = fcmp olt double %force_roundfr_rounded1518, %force_roundfr_rounded1521
  %mul_hi_cd_test1529 = fcmp olt double %force_roundfr_rounded1524, %force_roundfr_rounded1527
  %mul_hi_ab_max1530 = select i1 %mul_hi_ab_test1528, double %force_roundfr_rounded1521, double %force_roundfr_rounded1518
  %mul_hi_cd_max1531 = select i1 %mul_hi_cd_test1529, double %force_roundfr_rounded1527, double %force_roundfr_rounded1524
  %mul_hi_abcd_test1532 = fcmp olt double %mul_hi_ab_max1530, %mul_hi_cd_max1531
  %mul_hi_abcd_max1533 = select i1 %mul_hi_abcd_test1532, double %mul_hi_cd_max1531, double %mul_hi_ab_max1530
  call void @set_rounding_mode_down()
  %force_round_fr_mem1535 = alloca double, align 8
  %388 = bitcast double* %force_round_fr_mem1535 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %388)
  store volatile double %mul_hi_a1516, double* %force_round_fr_mem1535, align 8
  %force_roundfr_rounded1536 = load volatile double* %force_round_fr_mem1535, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %388)
  %force_round_fr_mem1538 = alloca double, align 8
  %389 = bitcast double* %force_round_fr_mem1538 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %389)
  store volatile double %mul_hi_b1519, double* %force_round_fr_mem1538, align 8
  %force_roundfr_rounded1539 = load volatile double* %force_round_fr_mem1538, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %389)
  %force_round_fr_mem1541 = alloca double, align 8
  %390 = bitcast double* %force_round_fr_mem1541 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %390)
  store volatile double %mul_hi_c1522, double* %force_round_fr_mem1541, align 8
  %force_roundfr_rounded1542 = load volatile double* %force_round_fr_mem1541, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %390)
  %force_round_fr_mem1544 = alloca double, align 8
  %391 = bitcast double* %force_round_fr_mem1544 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %391)
  store volatile double %mul_hi_d1525, double* %force_round_fr_mem1544, align 8
  %force_roundfr_rounded1545 = load volatile double* %force_round_fr_mem1544, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %391)
  %mul_lo_ab_test1546 = fcmp olt double %force_roundfr_rounded1539, %force_roundfr_rounded1536
  %mul_lo_cd_test1547 = fcmp olt double %force_roundfr_rounded1545, %force_roundfr_rounded1542
  %mul_lo_cd_min1548 = select i1 %mul_lo_cd_test1547, double %force_roundfr_rounded1545, double %force_roundfr_rounded1542
  %mul_lo_ab_min1549 = select i1 %mul_lo_ab_test1546, double %force_roundfr_rounded1539, double %force_roundfr_rounded1536
  %mul_lo_abcd_test1550 = fcmp olt double %mul_lo_cd_min1548, %mul_lo_ab_min1549
  %mul_lo_abcd_min1551 = select i1 %mul_lo_abcd_test1550, double %mul_lo_cd_min1548, double %mul_lo_ab_min1549
  %neg_lo1552 = fsub double -0.000000e+00, %mul_hi_abcd_max1533
  %neg_hi1553 = fsub double -0.000000e+00, %mul_lo_abcd_min1551
  call void @set_rounding_mode_up()
  %mul_hi_a1554 = fmul double %r_lo, %neg_lo1552
  %force_round_fr_mem1555 = alloca double, align 8
  %392 = bitcast double* %force_round_fr_mem1555 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %392)
  store volatile double %mul_hi_a1554, double* %force_round_fr_mem1555, align 8
  %force_roundfr_rounded1556 = load volatile double* %force_round_fr_mem1555, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %392)
  %mul_hi_b1557 = fmul double %r_lo, %neg_hi1553
  %force_round_fr_mem1558 = alloca double, align 8
  %393 = bitcast double* %force_round_fr_mem1558 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %393)
  store volatile double %mul_hi_b1557, double* %force_round_fr_mem1558, align 8
  %force_roundfr_rounded1559 = load volatile double* %force_round_fr_mem1558, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %393)
  %mul_hi_c1560 = fmul double %r_hi, %neg_lo1552
  %force_round_fr_mem1561 = alloca double, align 8
  %394 = bitcast double* %force_round_fr_mem1561 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %394)
  store volatile double %mul_hi_c1560, double* %force_round_fr_mem1561, align 8
  %force_roundfr_rounded1562 = load volatile double* %force_round_fr_mem1561, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %394)
  %mul_hi_d1563 = fmul double %r_hi, %neg_hi1553
  %force_round_fr_mem1564 = alloca double, align 8
  %395 = bitcast double* %force_round_fr_mem1564 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %395)
  store volatile double %mul_hi_d1563, double* %force_round_fr_mem1564, align 8
  %force_roundfr_rounded1565 = load volatile double* %force_round_fr_mem1564, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %395)
  %mul_hi_ab_test1566 = fcmp olt double %force_roundfr_rounded1556, %force_roundfr_rounded1559
  %mul_hi_cd_test1567 = fcmp olt double %force_roundfr_rounded1562, %force_roundfr_rounded1565
  %mul_hi_ab_max1568 = select i1 %mul_hi_ab_test1566, double %force_roundfr_rounded1559, double %force_roundfr_rounded1556
  %mul_hi_cd_max1569 = select i1 %mul_hi_cd_test1567, double %force_roundfr_rounded1565, double %force_roundfr_rounded1562
  %mul_hi_abcd_test1570 = fcmp olt double %mul_hi_ab_max1568, %mul_hi_cd_max1569
  %mul_hi_abcd_max1571 = select i1 %mul_hi_abcd_test1570, double %mul_hi_cd_max1569, double %mul_hi_ab_max1568
  call void @set_rounding_mode_down()
  %force_round_fr_mem1573 = alloca double, align 8
  %396 = bitcast double* %force_round_fr_mem1573 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %396)
  store volatile double %mul_hi_a1554, double* %force_round_fr_mem1573, align 8
  %force_roundfr_rounded1574 = load volatile double* %force_round_fr_mem1573, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %396)
  %force_round_fr_mem1576 = alloca double, align 8
  %397 = bitcast double* %force_round_fr_mem1576 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %397)
  store volatile double %mul_hi_b1557, double* %force_round_fr_mem1576, align 8
  %force_roundfr_rounded1577 = load volatile double* %force_round_fr_mem1576, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %397)
  %force_round_fr_mem1579 = alloca double, align 8
  %398 = bitcast double* %force_round_fr_mem1579 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %398)
  store volatile double %mul_hi_c1560, double* %force_round_fr_mem1579, align 8
  %force_roundfr_rounded1580 = load volatile double* %force_round_fr_mem1579, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %398)
  %force_round_fr_mem1582 = alloca double, align 8
  %399 = bitcast double* %force_round_fr_mem1582 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %399)
  store volatile double %mul_hi_d1563, double* %force_round_fr_mem1582, align 8
  %force_roundfr_rounded1583 = load volatile double* %force_round_fr_mem1582, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %399)
  %mul_lo_ab_test1584 = fcmp olt double %force_roundfr_rounded1577, %force_roundfr_rounded1574
  %mul_lo_cd_test1585 = fcmp olt double %force_roundfr_rounded1583, %force_roundfr_rounded1580
  %mul_lo_cd_min1586 = select i1 %mul_lo_cd_test1585, double %force_roundfr_rounded1583, double %force_roundfr_rounded1580
  %mul_lo_ab_min1587 = select i1 %mul_lo_ab_test1584, double %force_roundfr_rounded1577, double %force_roundfr_rounded1574
  %mul_lo_abcd_test1588 = fcmp olt double %mul_lo_cd_min1586, %mul_lo_ab_min1587
  %mul_lo_abcd_min1589 = select i1 %mul_lo_abcd_test1588, double %mul_lo_cd_min1586, double %mul_lo_ab_min1587
  %sub_lo1590 = fsub double %mul_lo_abcd_min1589, %s_hi
  %force_round_fr_mem1591 = alloca double, align 8
  %400 = bitcast double* %force_round_fr_mem1591 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %400)
  store volatile double %sub_lo1590, double* %force_round_fr_mem1591, align 8
  %force_roundfr_rounded1592 = load volatile double* %force_round_fr_mem1591, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %400)
  call void @set_rounding_mode_up()
  %sub_hi1593 = fsub double %mul_hi_abcd_max1571, %s_lo
  %force_round_fr_mem1594 = alloca double, align 8
  %401 = bitcast double* %force_round_fr_mem1594 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %401)
  store volatile double %sub_hi1593, double* %force_round_fr_mem1594, align 8
  %force_roundfr_rounded1595 = load volatile double* %force_round_fr_mem1594, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %401)
  call void @set_rounding_mode_down()
  %add_lo1598 = fadd double %force_roundfr_rounded1592, -5.800000e+01
  %force_round_fr_mem1599 = alloca double, align 8
  %402 = bitcast double* %force_round_fr_mem1599 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %402)
  store volatile double %add_lo1598, double* %force_round_fr_mem1599, align 8
  %force_roundfr_rounded1600 = load volatile double* %force_round_fr_mem1599, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %402)
  call void @set_rounding_mode_up()
  %add_hi1601 = fadd double %force_roundfr_rounded1595, -5.800000e+01
  %force_round_fr_mem1602 = alloca double, align 8
  %403 = bitcast double* %force_round_fr_mem1602 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %403)
  store volatile double %add_hi1601, double* %force_round_fr_mem1602, align 8
  %force_roundfr_rounded1603 = load volatile double* %force_round_fr_mem1602, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %403)
  call void @set_rounding_mode_down()
  %sub_lo1604 = fsub double %t_lo, %force_roundfr_rounded1603
  %force_round_fr_mem1605 = alloca double, align 8
  %404 = bitcast double* %force_round_fr_mem1605 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %404)
  store volatile double %sub_lo1604, double* %force_round_fr_mem1605, align 8
  %force_roundfr_rounded1606 = load volatile double* %force_round_fr_mem1605, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %404)
  call void @set_rounding_mode_up()
  %sub_hi1607 = fsub double %t_hi, %force_roundfr_rounded1600
  %force_round_fr_mem1608 = alloca double, align 8
  %405 = bitcast double* %force_round_fr_mem1608 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %405)
  store volatile double %sub_hi1607, double* %force_round_fr_mem1608, align 8
  %force_roundfr_rounded1609 = load volatile double* %force_round_fr_mem1608, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %405)
  %mul_hi_a1610 = fmul double %force_roundfr_rounded1606, %u_lo
  %force_round_fr_mem1611 = alloca double, align 8
  %406 = bitcast double* %force_round_fr_mem1611 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %406)
  store volatile double %mul_hi_a1610, double* %force_round_fr_mem1611, align 8
  %force_roundfr_rounded1612 = load volatile double* %force_round_fr_mem1611, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %406)
  %mul_hi_b1613 = fmul double %force_roundfr_rounded1606, %u_hi
  %force_round_fr_mem1614 = alloca double, align 8
  %407 = bitcast double* %force_round_fr_mem1614 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %407)
  store volatile double %mul_hi_b1613, double* %force_round_fr_mem1614, align 8
  %force_roundfr_rounded1615 = load volatile double* %force_round_fr_mem1614, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %407)
  %mul_hi_c1616 = fmul double %force_roundfr_rounded1609, %u_lo
  %force_round_fr_mem1617 = alloca double, align 8
  %408 = bitcast double* %force_round_fr_mem1617 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %408)
  store volatile double %mul_hi_c1616, double* %force_round_fr_mem1617, align 8
  %force_roundfr_rounded1618 = load volatile double* %force_round_fr_mem1617, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %408)
  %mul_hi_d1619 = fmul double %force_roundfr_rounded1609, %u_hi
  %force_round_fr_mem1620 = alloca double, align 8
  %409 = bitcast double* %force_round_fr_mem1620 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %409)
  store volatile double %mul_hi_d1619, double* %force_round_fr_mem1620, align 8
  %force_roundfr_rounded1621 = load volatile double* %force_round_fr_mem1620, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %409)
  %mul_hi_ab_test1622 = fcmp olt double %force_roundfr_rounded1612, %force_roundfr_rounded1615
  %mul_hi_cd_test1623 = fcmp olt double %force_roundfr_rounded1618, %force_roundfr_rounded1621
  %mul_hi_ab_max1624 = select i1 %mul_hi_ab_test1622, double %force_roundfr_rounded1615, double %force_roundfr_rounded1612
  %mul_hi_cd_max1625 = select i1 %mul_hi_cd_test1623, double %force_roundfr_rounded1621, double %force_roundfr_rounded1618
  %mul_hi_abcd_test1626 = fcmp olt double %mul_hi_ab_max1624, %mul_hi_cd_max1625
  %mul_hi_abcd_max1627 = select i1 %mul_hi_abcd_test1626, double %mul_hi_cd_max1625, double %mul_hi_ab_max1624
  call void @set_rounding_mode_down()
  %force_round_fr_mem1629 = alloca double, align 8
  %410 = bitcast double* %force_round_fr_mem1629 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %410)
  store volatile double %mul_hi_a1610, double* %force_round_fr_mem1629, align 8
  %force_roundfr_rounded1630 = load volatile double* %force_round_fr_mem1629, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %410)
  %force_round_fr_mem1632 = alloca double, align 8
  %411 = bitcast double* %force_round_fr_mem1632 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %411)
  store volatile double %mul_hi_b1613, double* %force_round_fr_mem1632, align 8
  %force_roundfr_rounded1633 = load volatile double* %force_round_fr_mem1632, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %411)
  %force_round_fr_mem1635 = alloca double, align 8
  %412 = bitcast double* %force_round_fr_mem1635 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %412)
  store volatile double %mul_hi_c1616, double* %force_round_fr_mem1635, align 8
  %force_roundfr_rounded1636 = load volatile double* %force_round_fr_mem1635, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %412)
  %force_round_fr_mem1638 = alloca double, align 8
  %413 = bitcast double* %force_round_fr_mem1638 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %413)
  store volatile double %mul_hi_d1619, double* %force_round_fr_mem1638, align 8
  %force_roundfr_rounded1639 = load volatile double* %force_round_fr_mem1638, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %413)
  %mul_lo_ab_test1640 = fcmp olt double %force_roundfr_rounded1633, %force_roundfr_rounded1630
  %mul_lo_cd_test1641 = fcmp olt double %force_roundfr_rounded1639, %force_roundfr_rounded1636
  %mul_lo_cd_min1642 = select i1 %mul_lo_cd_test1641, double %force_roundfr_rounded1639, double %force_roundfr_rounded1636
  %mul_lo_ab_min1643 = select i1 %mul_lo_ab_test1640, double %force_roundfr_rounded1633, double %force_roundfr_rounded1630
  %mul_lo_abcd_test1644 = fcmp olt double %mul_lo_cd_min1642, %mul_lo_ab_min1643
  %mul_lo_abcd_min1645 = select i1 %mul_lo_abcd_test1644, double %mul_lo_cd_min1642, double %mul_lo_ab_min1643
  call void @set_rounding_mode_up()
  %div_hi_a1648 = fdiv double %mul_lo_abcd_min1645, -2.100000e+01
  %force_round_fr_mem1649 = alloca double, align 8
  %414 = bitcast double* %force_round_fr_mem1649 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %414)
  store volatile double %div_hi_a1648, double* %force_round_fr_mem1649, align 8
  %force_roundfr_rounded1650 = load volatile double* %force_round_fr_mem1649, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %414)
  %force_round_fr_mem1652 = alloca double, align 8
  %415 = bitcast double* %force_round_fr_mem1652 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %415)
  store volatile double %div_hi_a1648, double* %force_round_fr_mem1652, align 8
  %force_roundfr_rounded1653 = load volatile double* %force_round_fr_mem1652, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %415)
  %div_hi_c1654 = fdiv double %mul_hi_abcd_max1627, -2.100000e+01
  %force_round_fr_mem1655 = alloca double, align 8
  %416 = bitcast double* %force_round_fr_mem1655 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %416)
  store volatile double %div_hi_c1654, double* %force_round_fr_mem1655, align 8
  %force_roundfr_rounded1656 = load volatile double* %force_round_fr_mem1655, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %416)
  %force_round_fr_mem1658 = alloca double, align 8
  %417 = bitcast double* %force_round_fr_mem1658 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %417)
  store volatile double %div_hi_c1654, double* %force_round_fr_mem1658, align 8
  %force_roundfr_rounded1659 = load volatile double* %force_round_fr_mem1658, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %417)
  %div_hi_ab_test1660 = fcmp olt double %force_roundfr_rounded1650, %force_roundfr_rounded1653
  %div_hi_cd_test1661 = fcmp olt double %force_roundfr_rounded1656, %force_roundfr_rounded1659
  %div_hi_ab_max1662 = select i1 %div_hi_ab_test1660, double %force_roundfr_rounded1653, double %force_roundfr_rounded1650
  %div_hi_cd_max1663 = select i1 %div_hi_cd_test1661, double %force_roundfr_rounded1659, double %force_roundfr_rounded1656
  %div_hi_abcd_test1664 = fcmp olt double %div_hi_ab_max1662, %div_hi_cd_max1663
  %div_hi_abcd_max1665 = select i1 %div_hi_abcd_test1664, double %div_hi_cd_max1663, double %div_hi_ab_max1662
  call void @set_rounding_mode_down()
  %force_round_fr_mem1667 = alloca double, align 8
  %418 = bitcast double* %force_round_fr_mem1667 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %418)
  store volatile double %div_hi_a1648, double* %force_round_fr_mem1667, align 8
  %force_roundfr_rounded1668 = load volatile double* %force_round_fr_mem1667, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %418)
  %force_round_fr_mem1670 = alloca double, align 8
  %419 = bitcast double* %force_round_fr_mem1670 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %419)
  store volatile double %div_hi_a1648, double* %force_round_fr_mem1670, align 8
  %force_roundfr_rounded1671 = load volatile double* %force_round_fr_mem1670, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %419)
  %force_round_fr_mem1673 = alloca double, align 8
  %420 = bitcast double* %force_round_fr_mem1673 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %420)
  store volatile double %div_hi_c1654, double* %force_round_fr_mem1673, align 8
  %force_roundfr_rounded1674 = load volatile double* %force_round_fr_mem1673, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %420)
  %force_round_fr_mem1676 = alloca double, align 8
  %421 = bitcast double* %force_round_fr_mem1676 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %421)
  store volatile double %div_hi_c1654, double* %force_round_fr_mem1676, align 8
  %force_roundfr_rounded1677 = load volatile double* %force_round_fr_mem1676, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %421)
  %div_lo_ab_test1678 = fcmp olt double %force_roundfr_rounded1671, %force_roundfr_rounded1668
  %div_lo_cd_test1679 = fcmp olt double %force_roundfr_rounded1677, %force_roundfr_rounded1674
  %div_lo_cd_min1680 = select i1 %div_lo_cd_test1679, double %force_roundfr_rounded1677, double %force_roundfr_rounded1674
  %div_lo_ab_min1681 = select i1 %div_lo_ab_test1678, double %force_roundfr_rounded1671, double %force_roundfr_rounded1668
  %div_lo_abcd_test1682 = fcmp olt double %div_lo_cd_min1680, %div_lo_ab_min1681
  %div_lo_abcd_min1683 = select i1 %div_lo_abcd_test1682, double %div_lo_cd_min1680, double %div_lo_ab_min1681
  %expt_even_are_both_pos_p1684 = fcmp ogt double %div_lo_abcd_min1683, 0.000000e+00
  call void @set_rounding_mode_up()
  br i1 %expt_even_are_both_pos_p1684, label %expt_even_both_are_pos1685, label %expt_even_both_are_not_pos1704

expt_even_both_are_pos1685:                       ; preds = %expt_even_merge1513
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x1686 = fmul double %div_lo_abcd_min1683, %div_lo_abcd_min1683
  %force_round_fr_mem1687 = alloca double, align 8
  %422 = bitcast double* %force_round_fr_mem1687 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %422)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1686, double* %force_round_fr_mem1687, align 8
  %force_roundfr_rounded1688 = load volatile double* %force_round_fr_mem1687, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %422)
  %expt_even_pos_block_res_lo_unroll_x1689 = fmul double %force_roundfr_rounded1688, %force_roundfr_rounded1688
  %force_round_fr_mem1690 = alloca double, align 8
  %423 = bitcast double* %force_round_fr_mem1690 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %423)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1689, double* %force_round_fr_mem1690, align 8
  %force_roundfr_rounded1691 = load volatile double* %force_round_fr_mem1690, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %423)
  %force_round_fr_mem1693 = alloca double, align 8
  %424 = bitcast double* %force_round_fr_mem1693 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %424)
  store volatile double %force_roundfr_rounded1691, double* %force_round_fr_mem1693, align 8
  %force_roundfr_rounded1694 = load volatile double* %force_round_fr_mem1693, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %424)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1695 = fmul double %div_hi_abcd_max1665, %div_hi_abcd_max1665
  %force_round_fr_mem1696 = alloca double, align 8
  %425 = bitcast double* %force_round_fr_mem1696 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %425)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1695, double* %force_round_fr_mem1696, align 8
  %force_roundfr_rounded1697 = load volatile double* %force_round_fr_mem1696, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %425)
  %expt_even_pos_block_res_hi_unroll_x1698 = fmul double %force_roundfr_rounded1697, %force_roundfr_rounded1697
  %force_round_fr_mem1699 = alloca double, align 8
  %426 = bitcast double* %force_round_fr_mem1699 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %426)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1698, double* %force_round_fr_mem1699, align 8
  %force_roundfr_rounded1700 = load volatile double* %force_round_fr_mem1699, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %426)
  %force_round_fr_mem1702 = alloca double, align 8
  %427 = bitcast double* %force_round_fr_mem1702 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %427)
  store volatile double %force_roundfr_rounded1700, double* %force_round_fr_mem1702, align 8
  %force_roundfr_rounded1703 = load volatile double* %force_round_fr_mem1702, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %427)
  br label %expt_even_merge1746

expt_even_both_are_not_pos1704:                   ; preds = %expt_even_merge1513
  %expt_even_are_both_neg_p1705 = fcmp olt double %div_hi_abcd_max1665, 0.000000e+00
  br i1 %expt_even_are_both_neg_p1705, label %expt_even_both_are_neg1706, label %expt_even_contains_zero1725

expt_even_both_are_neg1706:                       ; preds = %expt_even_both_are_not_pos1704
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x1707 = fmul double %div_hi_abcd_max1665, %div_hi_abcd_max1665
  %force_round_fr_mem1708 = alloca double, align 8
  %428 = bitcast double* %force_round_fr_mem1708 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %428)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1707, double* %force_round_fr_mem1708, align 8
  %force_roundfr_rounded1709 = load volatile double* %force_round_fr_mem1708, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %428)
  %expt_even_neg_block_res_lo_unroll_x1710 = fmul double %force_roundfr_rounded1709, %force_roundfr_rounded1709
  %force_round_fr_mem1711 = alloca double, align 8
  %429 = bitcast double* %force_round_fr_mem1711 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %429)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1710, double* %force_round_fr_mem1711, align 8
  %force_roundfr_rounded1712 = load volatile double* %force_round_fr_mem1711, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %429)
  %force_round_fr_mem1714 = alloca double, align 8
  %430 = bitcast double* %force_round_fr_mem1714 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %430)
  store volatile double %force_roundfr_rounded1712, double* %force_round_fr_mem1714, align 8
  %force_roundfr_rounded1715 = load volatile double* %force_round_fr_mem1714, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %430)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1716 = fmul double %div_lo_abcd_min1683, %div_lo_abcd_min1683
  %force_round_fr_mem1717 = alloca double, align 8
  %431 = bitcast double* %force_round_fr_mem1717 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %431)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1716, double* %force_round_fr_mem1717, align 8
  %force_roundfr_rounded1718 = load volatile double* %force_round_fr_mem1717, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %431)
  %expt_even_pos_block_res_hi_unroll_x1719 = fmul double %force_roundfr_rounded1718, %force_roundfr_rounded1718
  %force_round_fr_mem1720 = alloca double, align 8
  %432 = bitcast double* %force_round_fr_mem1720 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %432)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1719, double* %force_round_fr_mem1720, align 8
  %force_roundfr_rounded1721 = load volatile double* %force_round_fr_mem1720, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %432)
  %force_round_fr_mem1723 = alloca double, align 8
  %433 = bitcast double* %force_round_fr_mem1723 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %433)
  store volatile double %force_roundfr_rounded1721, double* %force_round_fr_mem1723, align 8
  %force_roundfr_rounded1724 = load volatile double* %force_round_fr_mem1723, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %433)
  br label %expt_even_merge1746

expt_even_contains_zero1725:                      ; preds = %expt_even_both_are_not_pos1704
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x1726 = fmul double %div_lo_abcd_min1683, %div_lo_abcd_min1683
  %force_round_fr_mem1727 = alloca double, align 8
  %434 = bitcast double* %force_round_fr_mem1727 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %434)
  store volatile double %expt_even_mid_block_lo__unroll_x1726, double* %force_round_fr_mem1727, align 8
  %force_roundfr_rounded1728 = load volatile double* %force_round_fr_mem1727, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %434)
  %expt_even_mid_block_lo__unroll_x1729 = fmul double %force_roundfr_rounded1728, %force_roundfr_rounded1728
  %force_round_fr_mem1730 = alloca double, align 8
  %435 = bitcast double* %force_round_fr_mem1730 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %435)
  store volatile double %expt_even_mid_block_lo__unroll_x1729, double* %force_round_fr_mem1730, align 8
  %force_roundfr_rounded1731 = load volatile double* %force_round_fr_mem1730, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %435)
  %force_round_fr_mem1733 = alloca double, align 8
  %436 = bitcast double* %force_round_fr_mem1733 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %436)
  store volatile double %force_roundfr_rounded1731, double* %force_round_fr_mem1733, align 8
  %force_roundfr_rounded1734 = load volatile double* %force_round_fr_mem1733, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %436)
  %expt_even_mid_block_hi__unroll_x1735 = fmul double %div_hi_abcd_max1665, %div_hi_abcd_max1665
  %force_round_fr_mem1736 = alloca double, align 8
  %437 = bitcast double* %force_round_fr_mem1736 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %437)
  store volatile double %expt_even_mid_block_hi__unroll_x1735, double* %force_round_fr_mem1736, align 8
  %force_roundfr_rounded1737 = load volatile double* %force_round_fr_mem1736, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %437)
  %expt_even_mid_block_hi__unroll_x1738 = fmul double %force_roundfr_rounded1737, %force_roundfr_rounded1737
  %force_round_fr_mem1739 = alloca double, align 8
  %438 = bitcast double* %force_round_fr_mem1739 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %438)
  store volatile double %expt_even_mid_block_hi__unroll_x1738, double* %force_round_fr_mem1739, align 8
  %force_roundfr_rounded1740 = load volatile double* %force_round_fr_mem1739, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %438)
  %force_round_fr_mem1742 = alloca double, align 8
  %439 = bitcast double* %force_round_fr_mem1742 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %439)
  store volatile double %force_roundfr_rounded1740, double* %force_round_fr_mem1742, align 8
  %force_roundfr_rounded1743 = load volatile double* %force_round_fr_mem1742, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %439)
  %expt_even_mid_block_res_hi_abtest1744 = fcmp olt double %force_roundfr_rounded1734, %force_roundfr_rounded1743
  %expt_even_mid_block_res_hi_abmax1745 = select i1 %expt_even_mid_block_res_hi_abtest1744, double %force_roundfr_rounded1743, double %force_roundfr_rounded1734
  br label %expt_even_merge1746

expt_even_merge1746:                              ; preds = %expt_even_contains_zero1725, %expt_even_both_are_neg1706, %expt_even_both_are_pos1685
  %expt_lo1747 = phi double [ %force_roundfr_rounded1694, %expt_even_both_are_pos1685 ], [ %force_roundfr_rounded1715, %expt_even_both_are_neg1706 ], [ 0.000000e+00, %expt_even_contains_zero1725 ]
  %expt_hi1748 = phi double [ %force_roundfr_rounded1703, %expt_even_both_are_pos1685 ], [ %force_roundfr_rounded1724, %expt_even_both_are_neg1706 ], [ %expt_even_mid_block_res_hi_abmax1745, %expt_even_contains_zero1725 ]
  call void @set_rounding_mode_down()
  %expt_odd_lo_unroll_x1749 = fmul double %expt_lo1747, %expt_lo1747
  %force_round_fr_mem1750 = alloca double, align 8
  %440 = bitcast double* %force_round_fr_mem1750 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %440)
  store volatile double %expt_odd_lo_unroll_x1749, double* %force_round_fr_mem1750, align 8
  %force_roundfr_rounded1751 = load volatile double* %force_round_fr_mem1750, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %440)
  %expt_odd_lopwr_do_y1752 = fmul double %force_roundfr_rounded1751, %expt_lo1747
  %force_round_fr_mem1753 = alloca double, align 8
  %441 = bitcast double* %force_round_fr_mem1753 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %441)
  store volatile double %expt_odd_lopwr_do_y1752, double* %force_round_fr_mem1753, align 8
  %force_roundfr_rounded1754 = load volatile double* %force_round_fr_mem1753, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %441)
  call void @set_rounding_mode_up()
  %expt_odd_hi_unroll_x1755 = fmul double %expt_hi1748, %expt_hi1748
  %force_round_fr_mem1756 = alloca double, align 8
  %442 = bitcast double* %force_round_fr_mem1756 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %442)
  store volatile double %expt_odd_hi_unroll_x1755, double* %force_round_fr_mem1756, align 8
  %force_roundfr_rounded1757 = load volatile double* %force_round_fr_mem1756, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %442)
  %expt_odd_hipwr_do_y1758 = fmul double %force_roundfr_rounded1757, %expt_hi1748
  %force_round_fr_mem1759 = alloca double, align 8
  %443 = bitcast double* %force_round_fr_mem1759 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %443)
  store volatile double %expt_odd_hipwr_do_y1758, double* %force_round_fr_mem1759, align 8
  %force_roundfr_rounded1760 = load volatile double* %force_round_fr_mem1759, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %443)
  %neg_lo1761 = fsub double -0.000000e+00, %force_roundfr_rounded1760
  %neg_hi1762 = fsub double -0.000000e+00, %force_roundfr_rounded1754
  %div_hi_a1763 = fdiv double %neg_lo1761, %v_lo
  %force_round_fr_mem1764 = alloca double, align 8
  %444 = bitcast double* %force_round_fr_mem1764 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %444)
  store volatile double %div_hi_a1763, double* %force_round_fr_mem1764, align 8
  %force_roundfr_rounded1765 = load volatile double* %force_round_fr_mem1764, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %444)
  %div_hi_b1766 = fdiv double %neg_lo1761, %v_hi
  %force_round_fr_mem1767 = alloca double, align 8
  %445 = bitcast double* %force_round_fr_mem1767 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %445)
  store volatile double %div_hi_b1766, double* %force_round_fr_mem1767, align 8
  %force_roundfr_rounded1768 = load volatile double* %force_round_fr_mem1767, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %445)
  %div_hi_c1769 = fdiv double %neg_hi1762, %v_lo
  %force_round_fr_mem1770 = alloca double, align 8
  %446 = bitcast double* %force_round_fr_mem1770 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %446)
  store volatile double %div_hi_c1769, double* %force_round_fr_mem1770, align 8
  %force_roundfr_rounded1771 = load volatile double* %force_round_fr_mem1770, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %446)
  %div_hi_d1772 = fdiv double %neg_hi1762, %v_hi
  %force_round_fr_mem1773 = alloca double, align 8
  %447 = bitcast double* %force_round_fr_mem1773 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %447)
  store volatile double %div_hi_d1772, double* %force_round_fr_mem1773, align 8
  %force_roundfr_rounded1774 = load volatile double* %force_round_fr_mem1773, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %447)
  %div_hi_ab_test1775 = fcmp olt double %force_roundfr_rounded1765, %force_roundfr_rounded1768
  %div_hi_cd_test1776 = fcmp olt double %force_roundfr_rounded1771, %force_roundfr_rounded1774
  %div_hi_ab_max1777 = select i1 %div_hi_ab_test1775, double %force_roundfr_rounded1768, double %force_roundfr_rounded1765
  %div_hi_cd_max1778 = select i1 %div_hi_cd_test1776, double %force_roundfr_rounded1774, double %force_roundfr_rounded1771
  %div_hi_abcd_test1779 = fcmp olt double %div_hi_ab_max1777, %div_hi_cd_max1778
  %div_hi_abcd_max1780 = select i1 %div_hi_abcd_test1779, double %div_hi_cd_max1778, double %div_hi_ab_max1777
  call void @set_rounding_mode_down()
  %force_round_fr_mem1782 = alloca double, align 8
  %448 = bitcast double* %force_round_fr_mem1782 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %448)
  store volatile double %div_hi_a1763, double* %force_round_fr_mem1782, align 8
  %force_roundfr_rounded1783 = load volatile double* %force_round_fr_mem1782, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %448)
  %force_round_fr_mem1785 = alloca double, align 8
  %449 = bitcast double* %force_round_fr_mem1785 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %449)
  store volatile double %div_hi_b1766, double* %force_round_fr_mem1785, align 8
  %force_roundfr_rounded1786 = load volatile double* %force_round_fr_mem1785, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %449)
  %force_round_fr_mem1788 = alloca double, align 8
  %450 = bitcast double* %force_round_fr_mem1788 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %450)
  store volatile double %div_hi_c1769, double* %force_round_fr_mem1788, align 8
  %force_roundfr_rounded1789 = load volatile double* %force_round_fr_mem1788, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %450)
  %force_round_fr_mem1791 = alloca double, align 8
  %451 = bitcast double* %force_round_fr_mem1791 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %451)
  store volatile double %div_hi_d1772, double* %force_round_fr_mem1791, align 8
  %force_roundfr_rounded1792 = load volatile double* %force_round_fr_mem1791, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %451)
  %div_lo_ab_test1793 = fcmp olt double %force_roundfr_rounded1786, %force_roundfr_rounded1783
  %div_lo_cd_test1794 = fcmp olt double %force_roundfr_rounded1792, %force_roundfr_rounded1789
  %div_lo_cd_min1795 = select i1 %div_lo_cd_test1794, double %force_roundfr_rounded1792, double %force_roundfr_rounded1789
  %div_lo_ab_min1796 = select i1 %div_lo_ab_test1793, double %force_roundfr_rounded1786, double %force_roundfr_rounded1783
  %div_lo_abcd_test1797 = fcmp olt double %div_lo_cd_min1795, %div_lo_ab_min1796
  %div_lo_abcd_min1798 = select i1 %div_lo_abcd_test1797, double %div_lo_cd_min1795, double %div_lo_ab_min1796
  call void @set_rounding_mode_up()
  %div_hi_a1799 = fmul double %div_lo_abcd_min1798, 1.562500e-02
  %force_round_fr_mem1800 = alloca double, align 8
  %452 = bitcast double* %force_round_fr_mem1800 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %452)
  store volatile double %div_hi_a1799, double* %force_round_fr_mem1800, align 8
  %force_roundfr_rounded1801 = load volatile double* %force_round_fr_mem1800, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %452)
  %force_round_fr_mem1803 = alloca double, align 8
  %453 = bitcast double* %force_round_fr_mem1803 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %453)
  store volatile double %div_hi_a1799, double* %force_round_fr_mem1803, align 8
  %force_roundfr_rounded1804 = load volatile double* %force_round_fr_mem1803, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %453)
  %div_hi_c1805 = fmul double %div_hi_abcd_max1780, 1.562500e-02
  %force_round_fr_mem1806 = alloca double, align 8
  %454 = bitcast double* %force_round_fr_mem1806 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %454)
  store volatile double %div_hi_c1805, double* %force_round_fr_mem1806, align 8
  %force_roundfr_rounded1807 = load volatile double* %force_round_fr_mem1806, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %454)
  %force_round_fr_mem1809 = alloca double, align 8
  %455 = bitcast double* %force_round_fr_mem1809 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %455)
  store volatile double %div_hi_c1805, double* %force_round_fr_mem1809, align 8
  %force_roundfr_rounded1810 = load volatile double* %force_round_fr_mem1809, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %455)
  %div_hi_ab_test1811 = fcmp olt double %force_roundfr_rounded1801, %force_roundfr_rounded1804
  %div_hi_cd_test1812 = fcmp olt double %force_roundfr_rounded1807, %force_roundfr_rounded1810
  %div_hi_ab_max1813 = select i1 %div_hi_ab_test1811, double %force_roundfr_rounded1804, double %force_roundfr_rounded1801
  %div_hi_cd_max1814 = select i1 %div_hi_cd_test1812, double %force_roundfr_rounded1810, double %force_roundfr_rounded1807
  %div_hi_abcd_test1815 = fcmp olt double %div_hi_ab_max1813, %div_hi_cd_max1814
  %div_hi_abcd_max1816 = select i1 %div_hi_abcd_test1815, double %div_hi_cd_max1814, double %div_hi_ab_max1813
  call void @set_rounding_mode_down()
  %force_round_fr_mem1818 = alloca double, align 8
  %456 = bitcast double* %force_round_fr_mem1818 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %456)
  store volatile double %div_hi_a1799, double* %force_round_fr_mem1818, align 8
  %force_roundfr_rounded1819 = load volatile double* %force_round_fr_mem1818, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %456)
  %force_round_fr_mem1821 = alloca double, align 8
  %457 = bitcast double* %force_round_fr_mem1821 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %457)
  store volatile double %div_hi_a1799, double* %force_round_fr_mem1821, align 8
  %force_roundfr_rounded1822 = load volatile double* %force_round_fr_mem1821, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %457)
  %force_round_fr_mem1824 = alloca double, align 8
  %458 = bitcast double* %force_round_fr_mem1824 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %458)
  store volatile double %div_hi_c1805, double* %force_round_fr_mem1824, align 8
  %force_roundfr_rounded1825 = load volatile double* %force_round_fr_mem1824, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %458)
  %force_round_fr_mem1827 = alloca double, align 8
  %459 = bitcast double* %force_round_fr_mem1827 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %459)
  store volatile double %div_hi_c1805, double* %force_round_fr_mem1827, align 8
  %force_roundfr_rounded1828 = load volatile double* %force_round_fr_mem1827, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %459)
  %div_lo_ab_test1829 = fcmp olt double %force_roundfr_rounded1822, %force_roundfr_rounded1819
  %div_lo_cd_test1830 = fcmp olt double %force_roundfr_rounded1828, %force_roundfr_rounded1825
  %div_lo_cd_min1831 = select i1 %div_lo_cd_test1830, double %force_roundfr_rounded1828, double %force_roundfr_rounded1825
  %div_lo_ab_min1832 = select i1 %div_lo_ab_test1829, double %force_roundfr_rounded1822, double %force_roundfr_rounded1819
  %div_lo_abcd_test1833 = fcmp olt double %div_lo_cd_min1831, %div_lo_ab_min1832
  %div_lo_abcd_min1834 = select i1 %div_lo_abcd_test1833, double %div_lo_cd_min1831, double %div_lo_ab_min1832
  %add_lo1835 = fadd double %div_lo_abcd_min1834, 7.300000e+01
  %force_round_fr_mem1836 = alloca double, align 8
  %460 = bitcast double* %force_round_fr_mem1836 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %460)
  store volatile double %add_lo1835, double* %force_round_fr_mem1836, align 8
  %force_roundfr_rounded1837 = load volatile double* %force_round_fr_mem1836, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %460)
  call void @set_rounding_mode_up()
  %add_hi1838 = fadd double %div_hi_abcd_max1816, 7.300000e+01
  %force_round_fr_mem1839 = alloca double, align 8
  %461 = bitcast double* %force_round_fr_mem1839 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %461)
  store volatile double %add_hi1838, double* %force_round_fr_mem1839, align 8
  %force_roundfr_rounded1840 = load volatile double* %force_round_fr_mem1839, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %461)
  call void @set_rounding_mode_down()
  %sub_lo1841 = fsub double %force_roundfr_rounded1837, %w_hi
  %force_round_fr_mem1842 = alloca double, align 8
  %462 = bitcast double* %force_round_fr_mem1842 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %462)
  store volatile double %sub_lo1841, double* %force_round_fr_mem1842, align 8
  %force_roundfr_rounded1843 = load volatile double* %force_round_fr_mem1842, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %462)
  call void @set_rounding_mode_up()
  %sub_hi1844 = fsub double %force_roundfr_rounded1840, %w_lo
  %force_round_fr_mem1845 = alloca double, align 8
  %463 = bitcast double* %force_round_fr_mem1845 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %463)
  store volatile double %sub_hi1844, double* %force_round_fr_mem1845, align 8
  %force_roundfr_rounded1846 = load volatile double* %force_round_fr_mem1845, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %463)
  call void @set_rounding_mode_down()
  %sub_lo1847 = fsub double %force_roundfr_rounded1843, %x_hi
  %force_round_fr_mem1848 = alloca double, align 8
  %464 = bitcast double* %force_round_fr_mem1848 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %464)
  store volatile double %sub_lo1847, double* %force_round_fr_mem1848, align 8
  %force_roundfr_rounded1849 = load volatile double* %force_round_fr_mem1848, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %464)
  call void @set_rounding_mode_up()
  %sub_hi1850 = fsub double %force_roundfr_rounded1846, %x_lo
  %force_round_fr_mem1851 = alloca double, align 8
  %465 = bitcast double* %force_round_fr_mem1851 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %465)
  store volatile double %sub_hi1850, double* %force_round_fr_mem1851, align 8
  %force_roundfr_rounded1852 = load volatile double* %force_round_fr_mem1851, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %465)
  call void @set_rounding_mode_down()
  %add_lo1853 = fadd double %force_roundfr_rounded1849, 1.200000e+01
  %force_round_fr_mem1854 = alloca double, align 8
  %466 = bitcast double* %force_round_fr_mem1854 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %466)
  store volatile double %add_lo1853, double* %force_round_fr_mem1854, align 8
  %force_roundfr_rounded1855 = load volatile double* %force_round_fr_mem1854, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %466)
  call void @set_rounding_mode_up()
  %add_hi1856 = fadd double %force_roundfr_rounded1852, 1.200000e+01
  %force_round_fr_mem1857 = alloca double, align 8
  %467 = bitcast double* %force_round_fr_mem1857 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %467)
  store volatile double %add_hi1856, double* %force_round_fr_mem1857, align 8
  %force_roundfr_rounded1858 = load volatile double* %force_round_fr_mem1857, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %467)
  %expt_even_are_both_pos_p1861 = fcmp olt double %force_roundfr_rounded1858, -0.000000e+00
  br i1 %expt_even_are_both_pos_p1861, label %expt_even_both_are_pos1862, label %expt_even_both_are_not_pos1881

expt_even_both_are_pos1862:                       ; preds = %expt_even_merge1746
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x1863 = fmul double %force_roundfr_rounded1858, %force_roundfr_rounded1858
  %force_round_fr_mem1864 = alloca double, align 8
  %468 = bitcast double* %force_round_fr_mem1864 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %468)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1863, double* %force_round_fr_mem1864, align 8
  %force_roundfr_rounded1865 = load volatile double* %force_round_fr_mem1864, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %468)
  %expt_even_pos_block_res_lo_unroll_x1866 = fmul double %force_roundfr_rounded1865, %force_roundfr_rounded1865
  %force_round_fr_mem1867 = alloca double, align 8
  %469 = bitcast double* %force_round_fr_mem1867 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %469)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1866, double* %force_round_fr_mem1867, align 8
  %force_roundfr_rounded1868 = load volatile double* %force_round_fr_mem1867, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %469)
  %force_round_fr_mem1870 = alloca double, align 8
  %470 = bitcast double* %force_round_fr_mem1870 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %470)
  store volatile double %force_roundfr_rounded1868, double* %force_round_fr_mem1870, align 8
  %force_roundfr_rounded1871 = load volatile double* %force_round_fr_mem1870, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %470)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1872 = fmul double %force_roundfr_rounded1855, %force_roundfr_rounded1855
  %force_round_fr_mem1873 = alloca double, align 8
  %471 = bitcast double* %force_round_fr_mem1873 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %471)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1872, double* %force_round_fr_mem1873, align 8
  %force_roundfr_rounded1874 = load volatile double* %force_round_fr_mem1873, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %471)
  %expt_even_pos_block_res_hi_unroll_x1875 = fmul double %force_roundfr_rounded1874, %force_roundfr_rounded1874
  %force_round_fr_mem1876 = alloca double, align 8
  %472 = bitcast double* %force_round_fr_mem1876 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %472)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1875, double* %force_round_fr_mem1876, align 8
  %force_roundfr_rounded1877 = load volatile double* %force_round_fr_mem1876, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %472)
  %force_round_fr_mem1879 = alloca double, align 8
  %473 = bitcast double* %force_round_fr_mem1879 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %473)
  store volatile double %force_roundfr_rounded1877, double* %force_round_fr_mem1879, align 8
  %force_roundfr_rounded1880 = load volatile double* %force_round_fr_mem1879, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %473)
  br label %expt_even_merge1923

expt_even_both_are_not_pos1881:                   ; preds = %expt_even_merge1746
  %expt_even_are_both_neg_p1882 = fcmp ogt double %force_roundfr_rounded1855, -0.000000e+00
  br i1 %expt_even_are_both_neg_p1882, label %expt_even_both_are_neg1883, label %expt_even_contains_zero1902

expt_even_both_are_neg1883:                       ; preds = %expt_even_both_are_not_pos1881
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x1884 = fmul double %force_roundfr_rounded1855, %force_roundfr_rounded1855
  %force_round_fr_mem1885 = alloca double, align 8
  %474 = bitcast double* %force_round_fr_mem1885 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %474)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1884, double* %force_round_fr_mem1885, align 8
  %force_roundfr_rounded1886 = load volatile double* %force_round_fr_mem1885, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %474)
  %expt_even_neg_block_res_lo_unroll_x1887 = fmul double %force_roundfr_rounded1886, %force_roundfr_rounded1886
  %force_round_fr_mem1888 = alloca double, align 8
  %475 = bitcast double* %force_round_fr_mem1888 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %475)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1887, double* %force_round_fr_mem1888, align 8
  %force_roundfr_rounded1889 = load volatile double* %force_round_fr_mem1888, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %475)
  %force_round_fr_mem1891 = alloca double, align 8
  %476 = bitcast double* %force_round_fr_mem1891 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %476)
  store volatile double %force_roundfr_rounded1889, double* %force_round_fr_mem1891, align 8
  %force_roundfr_rounded1892 = load volatile double* %force_round_fr_mem1891, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %476)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1893 = fmul double %force_roundfr_rounded1858, %force_roundfr_rounded1858
  %force_round_fr_mem1894 = alloca double, align 8
  %477 = bitcast double* %force_round_fr_mem1894 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %477)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1893, double* %force_round_fr_mem1894, align 8
  %force_roundfr_rounded1895 = load volatile double* %force_round_fr_mem1894, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %477)
  %expt_even_pos_block_res_hi_unroll_x1896 = fmul double %force_roundfr_rounded1895, %force_roundfr_rounded1895
  %force_round_fr_mem1897 = alloca double, align 8
  %478 = bitcast double* %force_round_fr_mem1897 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %478)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1896, double* %force_round_fr_mem1897, align 8
  %force_roundfr_rounded1898 = load volatile double* %force_round_fr_mem1897, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %478)
  %force_round_fr_mem1900 = alloca double, align 8
  %479 = bitcast double* %force_round_fr_mem1900 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %479)
  store volatile double %force_roundfr_rounded1898, double* %force_round_fr_mem1900, align 8
  %force_roundfr_rounded1901 = load volatile double* %force_round_fr_mem1900, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %479)
  br label %expt_even_merge1923

expt_even_contains_zero1902:                      ; preds = %expt_even_both_are_not_pos1881
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x1903 = fmul double %force_roundfr_rounded1858, %force_roundfr_rounded1858
  %force_round_fr_mem1904 = alloca double, align 8
  %480 = bitcast double* %force_round_fr_mem1904 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %480)
  store volatile double %expt_even_mid_block_lo__unroll_x1903, double* %force_round_fr_mem1904, align 8
  %force_roundfr_rounded1905 = load volatile double* %force_round_fr_mem1904, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %480)
  %expt_even_mid_block_lo__unroll_x1906 = fmul double %force_roundfr_rounded1905, %force_roundfr_rounded1905
  %force_round_fr_mem1907 = alloca double, align 8
  %481 = bitcast double* %force_round_fr_mem1907 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %481)
  store volatile double %expt_even_mid_block_lo__unroll_x1906, double* %force_round_fr_mem1907, align 8
  %force_roundfr_rounded1908 = load volatile double* %force_round_fr_mem1907, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %481)
  %force_round_fr_mem1910 = alloca double, align 8
  %482 = bitcast double* %force_round_fr_mem1910 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %482)
  store volatile double %force_roundfr_rounded1908, double* %force_round_fr_mem1910, align 8
  %force_roundfr_rounded1911 = load volatile double* %force_round_fr_mem1910, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %482)
  %expt_even_mid_block_hi__unroll_x1912 = fmul double %force_roundfr_rounded1855, %force_roundfr_rounded1855
  %force_round_fr_mem1913 = alloca double, align 8
  %483 = bitcast double* %force_round_fr_mem1913 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %483)
  store volatile double %expt_even_mid_block_hi__unroll_x1912, double* %force_round_fr_mem1913, align 8
  %force_roundfr_rounded1914 = load volatile double* %force_round_fr_mem1913, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %483)
  %expt_even_mid_block_hi__unroll_x1915 = fmul double %force_roundfr_rounded1914, %force_roundfr_rounded1914
  %force_round_fr_mem1916 = alloca double, align 8
  %484 = bitcast double* %force_round_fr_mem1916 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %484)
  store volatile double %expt_even_mid_block_hi__unroll_x1915, double* %force_round_fr_mem1916, align 8
  %force_roundfr_rounded1917 = load volatile double* %force_round_fr_mem1916, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %484)
  %force_round_fr_mem1919 = alloca double, align 8
  %485 = bitcast double* %force_round_fr_mem1919 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %485)
  store volatile double %force_roundfr_rounded1917, double* %force_round_fr_mem1919, align 8
  %force_roundfr_rounded1920 = load volatile double* %force_round_fr_mem1919, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %485)
  %expt_even_mid_block_res_hi_abtest1921 = fcmp olt double %force_roundfr_rounded1911, %force_roundfr_rounded1920
  %expt_even_mid_block_res_hi_abmax1922 = select i1 %expt_even_mid_block_res_hi_abtest1921, double %force_roundfr_rounded1920, double %force_roundfr_rounded1911
  br label %expt_even_merge1923

expt_even_merge1923:                              ; preds = %expt_even_contains_zero1902, %expt_even_both_are_neg1883, %expt_even_both_are_pos1862
  %expt_lo1924 = phi double [ %force_roundfr_rounded1871, %expt_even_both_are_pos1862 ], [ %force_roundfr_rounded1892, %expt_even_both_are_neg1883 ], [ 0.000000e+00, %expt_even_contains_zero1902 ]
  %expt_hi1925 = phi double [ %force_roundfr_rounded1880, %expt_even_both_are_pos1862 ], [ %force_roundfr_rounded1901, %expt_even_both_are_neg1883 ], [ %expt_even_mid_block_res_hi_abmax1922, %expt_even_contains_zero1902 ]
  %expt_even_are_both_pos_p1926 = fcmp ogt double %expt_lo1924, 0.000000e+00
  br i1 %expt_even_are_both_pos_p1926, label %expt_even_both_are_pos1927, label %expt_even_both_are_not_pos1940

expt_even_both_are_pos1927:                       ; preds = %expt_even_merge1923
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x1928 = fmul double %expt_lo1924, %expt_lo1924
  %force_round_fr_mem1929 = alloca double, align 8
  %486 = bitcast double* %force_round_fr_mem1929 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %486)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1928, double* %force_round_fr_mem1929, align 8
  %force_roundfr_rounded1930 = load volatile double* %force_round_fr_mem1929, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %486)
  %force_round_fr_mem1932 = alloca double, align 8
  %487 = bitcast double* %force_round_fr_mem1932 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %487)
  store volatile double %force_roundfr_rounded1930, double* %force_round_fr_mem1932, align 8
  %force_roundfr_rounded1933 = load volatile double* %force_round_fr_mem1932, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %487)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1934 = fmul double %expt_hi1925, %expt_hi1925
  %force_round_fr_mem1935 = alloca double, align 8
  %488 = bitcast double* %force_round_fr_mem1935 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %488)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1934, double* %force_round_fr_mem1935, align 8
  %force_roundfr_rounded1936 = load volatile double* %force_round_fr_mem1935, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %488)
  %force_round_fr_mem1938 = alloca double, align 8
  %489 = bitcast double* %force_round_fr_mem1938 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %489)
  store volatile double %force_roundfr_rounded1936, double* %force_round_fr_mem1938, align 8
  %force_roundfr_rounded1939 = load volatile double* %force_round_fr_mem1938, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %489)
  br label %expt_even_merge1970

expt_even_both_are_not_pos1940:                   ; preds = %expt_even_merge1923
  %expt_even_are_both_neg_p1941 = fcmp olt double %expt_hi1925, 0.000000e+00
  br i1 %expt_even_are_both_neg_p1941, label %expt_even_both_are_neg1942, label %expt_even_contains_zero1955

expt_even_both_are_neg1942:                       ; preds = %expt_even_both_are_not_pos1940
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x1943 = fmul double %expt_hi1925, %expt_hi1925
  %force_round_fr_mem1944 = alloca double, align 8
  %490 = bitcast double* %force_round_fr_mem1944 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %490)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1943, double* %force_round_fr_mem1944, align 8
  %force_roundfr_rounded1945 = load volatile double* %force_round_fr_mem1944, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %490)
  %force_round_fr_mem1947 = alloca double, align 8
  %491 = bitcast double* %force_round_fr_mem1947 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %491)
  store volatile double %force_roundfr_rounded1945, double* %force_round_fr_mem1947, align 8
  %force_roundfr_rounded1948 = load volatile double* %force_round_fr_mem1947, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %491)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1949 = fmul double %expt_lo1924, %expt_lo1924
  %force_round_fr_mem1950 = alloca double, align 8
  %492 = bitcast double* %force_round_fr_mem1950 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %492)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1949, double* %force_round_fr_mem1950, align 8
  %force_roundfr_rounded1951 = load volatile double* %force_round_fr_mem1950, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %492)
  %force_round_fr_mem1953 = alloca double, align 8
  %493 = bitcast double* %force_round_fr_mem1953 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %493)
  store volatile double %force_roundfr_rounded1951, double* %force_round_fr_mem1953, align 8
  %force_roundfr_rounded1954 = load volatile double* %force_round_fr_mem1953, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %493)
  br label %expt_even_merge1970

expt_even_contains_zero1955:                      ; preds = %expt_even_both_are_not_pos1940
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x1956 = fmul double %expt_lo1924, %expt_lo1924
  %force_round_fr_mem1957 = alloca double, align 8
  %494 = bitcast double* %force_round_fr_mem1957 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %494)
  store volatile double %expt_even_mid_block_lo__unroll_x1956, double* %force_round_fr_mem1957, align 8
  %force_roundfr_rounded1958 = load volatile double* %force_round_fr_mem1957, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %494)
  %force_round_fr_mem1960 = alloca double, align 8
  %495 = bitcast double* %force_round_fr_mem1960 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %495)
  store volatile double %force_roundfr_rounded1958, double* %force_round_fr_mem1960, align 8
  %force_roundfr_rounded1961 = load volatile double* %force_round_fr_mem1960, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %495)
  %expt_even_mid_block_hi__unroll_x1962 = fmul double %expt_hi1925, %expt_hi1925
  %force_round_fr_mem1963 = alloca double, align 8
  %496 = bitcast double* %force_round_fr_mem1963 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %496)
  store volatile double %expt_even_mid_block_hi__unroll_x1962, double* %force_round_fr_mem1963, align 8
  %force_roundfr_rounded1964 = load volatile double* %force_round_fr_mem1963, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %496)
  %force_round_fr_mem1966 = alloca double, align 8
  %497 = bitcast double* %force_round_fr_mem1966 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %497)
  store volatile double %force_roundfr_rounded1964, double* %force_round_fr_mem1966, align 8
  %force_roundfr_rounded1967 = load volatile double* %force_round_fr_mem1966, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %497)
  %expt_even_mid_block_res_hi_abtest1968 = fcmp olt double %force_roundfr_rounded1961, %force_roundfr_rounded1967
  %expt_even_mid_block_res_hi_abmax1969 = select i1 %expt_even_mid_block_res_hi_abtest1968, double %force_roundfr_rounded1967, double %force_roundfr_rounded1961
  br label %expt_even_merge1970

expt_even_merge1970:                              ; preds = %expt_even_contains_zero1955, %expt_even_both_are_neg1942, %expt_even_both_are_pos1927
  %expt_lo1971 = phi double [ %force_roundfr_rounded1933, %expt_even_both_are_pos1927 ], [ %force_roundfr_rounded1948, %expt_even_both_are_neg1942 ], [ 0.000000e+00, %expt_even_contains_zero1955 ]
  %expt_hi1972 = phi double [ %force_roundfr_rounded1939, %expt_even_both_are_pos1927 ], [ %force_roundfr_rounded1954, %expt_even_both_are_neg1942 ], [ %expt_even_mid_block_res_hi_abmax1969, %expt_even_contains_zero1955 ]
  call void @set_rounding_mode_down()
  %sub_lo1975 = fadd double %expt_lo1971, 3.300000e+01
  %force_round_fr_mem1976 = alloca double, align 8
  %498 = bitcast double* %force_round_fr_mem1976 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %498)
  store volatile double %sub_lo1975, double* %force_round_fr_mem1976, align 8
  %force_roundfr_rounded1977 = load volatile double* %force_round_fr_mem1976, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %498)
  call void @set_rounding_mode_up()
  %sub_hi1978 = fadd double %expt_hi1972, 3.300000e+01
  %force_round_fr_mem1979 = alloca double, align 8
  %499 = bitcast double* %force_round_fr_mem1979 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %499)
  store volatile double %sub_hi1978, double* %force_round_fr_mem1979, align 8
  %force_roundfr_rounded1980 = load volatile double* %force_round_fr_mem1979, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %499)
  %expt_even_are_both_pos_p1981 = fcmp ogt double %force_roundfr_rounded1977, 0.000000e+00
  br i1 %expt_even_are_both_pos_p1981, label %expt_even_both_are_pos1982, label %expt_even_both_are_not_pos1995

expt_even_both_are_pos1982:                       ; preds = %expt_even_merge1970
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x1983 = fmul double %force_roundfr_rounded1977, %force_roundfr_rounded1977
  %force_round_fr_mem1984 = alloca double, align 8
  %500 = bitcast double* %force_round_fr_mem1984 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %500)
  store volatile double %expt_even_pos_block_res_lo_unroll_x1983, double* %force_round_fr_mem1984, align 8
  %force_roundfr_rounded1985 = load volatile double* %force_round_fr_mem1984, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %500)
  %force_round_fr_mem1987 = alloca double, align 8
  %501 = bitcast double* %force_round_fr_mem1987 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %501)
  store volatile double %force_roundfr_rounded1985, double* %force_round_fr_mem1987, align 8
  %force_roundfr_rounded1988 = load volatile double* %force_round_fr_mem1987, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %501)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x1989 = fmul double %force_roundfr_rounded1980, %force_roundfr_rounded1980
  %force_round_fr_mem1990 = alloca double, align 8
  %502 = bitcast double* %force_round_fr_mem1990 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %502)
  store volatile double %expt_even_pos_block_res_hi_unroll_x1989, double* %force_round_fr_mem1990, align 8
  %force_roundfr_rounded1991 = load volatile double* %force_round_fr_mem1990, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %502)
  %force_round_fr_mem1993 = alloca double, align 8
  %503 = bitcast double* %force_round_fr_mem1993 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %503)
  store volatile double %force_roundfr_rounded1991, double* %force_round_fr_mem1993, align 8
  %force_roundfr_rounded1994 = load volatile double* %force_round_fr_mem1993, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %503)
  br label %expt_even_merge2025

expt_even_both_are_not_pos1995:                   ; preds = %expt_even_merge1970
  %expt_even_are_both_neg_p1996 = fcmp olt double %force_roundfr_rounded1980, 0.000000e+00
  br i1 %expt_even_are_both_neg_p1996, label %expt_even_both_are_neg1997, label %expt_even_contains_zero2010

expt_even_both_are_neg1997:                       ; preds = %expt_even_both_are_not_pos1995
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x1998 = fmul double %force_roundfr_rounded1980, %force_roundfr_rounded1980
  %force_round_fr_mem1999 = alloca double, align 8
  %504 = bitcast double* %force_round_fr_mem1999 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %504)
  store volatile double %expt_even_neg_block_res_lo_unroll_x1998, double* %force_round_fr_mem1999, align 8
  %force_roundfr_rounded2000 = load volatile double* %force_round_fr_mem1999, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %504)
  %force_round_fr_mem2002 = alloca double, align 8
  %505 = bitcast double* %force_round_fr_mem2002 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %505)
  store volatile double %force_roundfr_rounded2000, double* %force_round_fr_mem2002, align 8
  %force_roundfr_rounded2003 = load volatile double* %force_round_fr_mem2002, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %505)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x2004 = fmul double %force_roundfr_rounded1977, %force_roundfr_rounded1977
  %force_round_fr_mem2005 = alloca double, align 8
  %506 = bitcast double* %force_round_fr_mem2005 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %506)
  store volatile double %expt_even_pos_block_res_hi_unroll_x2004, double* %force_round_fr_mem2005, align 8
  %force_roundfr_rounded2006 = load volatile double* %force_round_fr_mem2005, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %506)
  %force_round_fr_mem2008 = alloca double, align 8
  %507 = bitcast double* %force_round_fr_mem2008 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %507)
  store volatile double %force_roundfr_rounded2006, double* %force_round_fr_mem2008, align 8
  %force_roundfr_rounded2009 = load volatile double* %force_round_fr_mem2008, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %507)
  br label %expt_even_merge2025

expt_even_contains_zero2010:                      ; preds = %expt_even_both_are_not_pos1995
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x2011 = fmul double %force_roundfr_rounded1977, %force_roundfr_rounded1977
  %force_round_fr_mem2012 = alloca double, align 8
  %508 = bitcast double* %force_round_fr_mem2012 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %508)
  store volatile double %expt_even_mid_block_lo__unroll_x2011, double* %force_round_fr_mem2012, align 8
  %force_roundfr_rounded2013 = load volatile double* %force_round_fr_mem2012, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %508)
  %force_round_fr_mem2015 = alloca double, align 8
  %509 = bitcast double* %force_round_fr_mem2015 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %509)
  store volatile double %force_roundfr_rounded2013, double* %force_round_fr_mem2015, align 8
  %force_roundfr_rounded2016 = load volatile double* %force_round_fr_mem2015, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %509)
  %expt_even_mid_block_hi__unroll_x2017 = fmul double %force_roundfr_rounded1980, %force_roundfr_rounded1980
  %force_round_fr_mem2018 = alloca double, align 8
  %510 = bitcast double* %force_round_fr_mem2018 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %510)
  store volatile double %expt_even_mid_block_hi__unroll_x2017, double* %force_round_fr_mem2018, align 8
  %force_roundfr_rounded2019 = load volatile double* %force_round_fr_mem2018, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %510)
  %force_round_fr_mem2021 = alloca double, align 8
  %511 = bitcast double* %force_round_fr_mem2021 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %511)
  store volatile double %force_roundfr_rounded2019, double* %force_round_fr_mem2021, align 8
  %force_roundfr_rounded2022 = load volatile double* %force_round_fr_mem2021, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %511)
  %expt_even_mid_block_res_hi_abtest2023 = fcmp olt double %force_roundfr_rounded2016, %force_roundfr_rounded2022
  %expt_even_mid_block_res_hi_abmax2024 = select i1 %expt_even_mid_block_res_hi_abtest2023, double %force_roundfr_rounded2022, double %force_roundfr_rounded2016
  br label %expt_even_merge2025

expt_even_merge2025:                              ; preds = %expt_even_contains_zero2010, %expt_even_both_are_neg1997, %expt_even_both_are_pos1982
  %expt_lo2026 = phi double [ %force_roundfr_rounded1988, %expt_even_both_are_pos1982 ], [ %force_roundfr_rounded2003, %expt_even_both_are_neg1997 ], [ 0.000000e+00, %expt_even_contains_zero2010 ]
  %expt_hi2027 = phi double [ %force_roundfr_rounded1994, %expt_even_both_are_pos1982 ], [ %force_roundfr_rounded2009, %expt_even_both_are_neg1997 ], [ %expt_even_mid_block_res_hi_abmax2024, %expt_even_contains_zero2010 ]
  %mul_hi_a2030 = fmul double %expt_lo2026, -1.000000e+02
  %force_round_fr_mem2031 = alloca double, align 8
  %512 = bitcast double* %force_round_fr_mem2031 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %512)
  store volatile double %mul_hi_a2030, double* %force_round_fr_mem2031, align 8
  %force_roundfr_rounded2032 = load volatile double* %force_round_fr_mem2031, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %512)
  %force_round_fr_mem2034 = alloca double, align 8
  %513 = bitcast double* %force_round_fr_mem2034 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %513)
  store volatile double %mul_hi_a2030, double* %force_round_fr_mem2034, align 8
  %force_roundfr_rounded2035 = load volatile double* %force_round_fr_mem2034, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %513)
  %mul_hi_c2036 = fmul double %expt_hi2027, -1.000000e+02
  %force_round_fr_mem2037 = alloca double, align 8
  %514 = bitcast double* %force_round_fr_mem2037 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %514)
  store volatile double %mul_hi_c2036, double* %force_round_fr_mem2037, align 8
  %force_roundfr_rounded2038 = load volatile double* %force_round_fr_mem2037, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %514)
  %force_round_fr_mem2040 = alloca double, align 8
  %515 = bitcast double* %force_round_fr_mem2040 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %515)
  store volatile double %mul_hi_c2036, double* %force_round_fr_mem2040, align 8
  %force_roundfr_rounded2041 = load volatile double* %force_round_fr_mem2040, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %515)
  %mul_hi_ab_test2042 = fcmp olt double %force_roundfr_rounded2032, %force_roundfr_rounded2035
  %mul_hi_cd_test2043 = fcmp olt double %force_roundfr_rounded2038, %force_roundfr_rounded2041
  %mul_hi_ab_max2044 = select i1 %mul_hi_ab_test2042, double %force_roundfr_rounded2035, double %force_roundfr_rounded2032
  %mul_hi_cd_max2045 = select i1 %mul_hi_cd_test2043, double %force_roundfr_rounded2041, double %force_roundfr_rounded2038
  %mul_hi_abcd_test2046 = fcmp olt double %mul_hi_ab_max2044, %mul_hi_cd_max2045
  %mul_hi_abcd_max2047 = select i1 %mul_hi_abcd_test2046, double %mul_hi_cd_max2045, double %mul_hi_ab_max2044
  call void @set_rounding_mode_down()
  %force_round_fr_mem2049 = alloca double, align 8
  %516 = bitcast double* %force_round_fr_mem2049 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %516)
  store volatile double %mul_hi_a2030, double* %force_round_fr_mem2049, align 8
  %force_roundfr_rounded2050 = load volatile double* %force_round_fr_mem2049, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %516)
  %force_round_fr_mem2052 = alloca double, align 8
  %517 = bitcast double* %force_round_fr_mem2052 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %517)
  store volatile double %mul_hi_a2030, double* %force_round_fr_mem2052, align 8
  %force_roundfr_rounded2053 = load volatile double* %force_round_fr_mem2052, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %517)
  %force_round_fr_mem2055 = alloca double, align 8
  %518 = bitcast double* %force_round_fr_mem2055 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %518)
  store volatile double %mul_hi_c2036, double* %force_round_fr_mem2055, align 8
  %force_roundfr_rounded2056 = load volatile double* %force_round_fr_mem2055, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %518)
  %force_round_fr_mem2058 = alloca double, align 8
  %519 = bitcast double* %force_round_fr_mem2058 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %519)
  store volatile double %mul_hi_c2036, double* %force_round_fr_mem2058, align 8
  %force_roundfr_rounded2059 = load volatile double* %force_round_fr_mem2058, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %519)
  %mul_lo_ab_test2060 = fcmp olt double %force_roundfr_rounded2053, %force_roundfr_rounded2050
  %mul_lo_cd_test2061 = fcmp olt double %force_roundfr_rounded2059, %force_roundfr_rounded2056
  %mul_lo_cd_min2062 = select i1 %mul_lo_cd_test2061, double %force_roundfr_rounded2059, double %force_roundfr_rounded2056
  %mul_lo_ab_min2063 = select i1 %mul_lo_ab_test2060, double %force_roundfr_rounded2053, double %force_roundfr_rounded2050
  %mul_lo_abcd_test2064 = fcmp olt double %mul_lo_cd_min2062, %mul_lo_ab_min2063
  %mul_lo_abcd_min2065 = select i1 %mul_lo_abcd_test2064, double %mul_lo_cd_min2062, double %mul_lo_ab_min2063
  %expt_even_are_both_pos_p2066 = fcmp ogt double %mul_lo_abcd_min2065, 0.000000e+00
  call void @set_rounding_mode_up()
  br i1 %expt_even_are_both_pos_p2066, label %expt_even_both_are_pos2067, label %expt_even_both_are_not_pos2086

expt_even_both_are_pos2067:                       ; preds = %expt_even_merge2025
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x2068 = fmul double %mul_lo_abcd_min2065, %mul_lo_abcd_min2065
  %force_round_fr_mem2069 = alloca double, align 8
  %520 = bitcast double* %force_round_fr_mem2069 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %520)
  store volatile double %expt_even_pos_block_res_lo_unroll_x2068, double* %force_round_fr_mem2069, align 8
  %force_roundfr_rounded2070 = load volatile double* %force_round_fr_mem2069, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %520)
  %expt_even_pos_block_res_lo_unroll_x2071 = fmul double %force_roundfr_rounded2070, %force_roundfr_rounded2070
  %force_round_fr_mem2072 = alloca double, align 8
  %521 = bitcast double* %force_round_fr_mem2072 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %521)
  store volatile double %expt_even_pos_block_res_lo_unroll_x2071, double* %force_round_fr_mem2072, align 8
  %force_roundfr_rounded2073 = load volatile double* %force_round_fr_mem2072, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %521)
  %force_round_fr_mem2075 = alloca double, align 8
  %522 = bitcast double* %force_round_fr_mem2075 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %522)
  store volatile double %force_roundfr_rounded2073, double* %force_round_fr_mem2075, align 8
  %force_roundfr_rounded2076 = load volatile double* %force_round_fr_mem2075, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %522)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x2077 = fmul double %mul_hi_abcd_max2047, %mul_hi_abcd_max2047
  %force_round_fr_mem2078 = alloca double, align 8
  %523 = bitcast double* %force_round_fr_mem2078 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %523)
  store volatile double %expt_even_pos_block_res_hi_unroll_x2077, double* %force_round_fr_mem2078, align 8
  %force_roundfr_rounded2079 = load volatile double* %force_round_fr_mem2078, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %523)
  %expt_even_pos_block_res_hi_unroll_x2080 = fmul double %force_roundfr_rounded2079, %force_roundfr_rounded2079
  %force_round_fr_mem2081 = alloca double, align 8
  %524 = bitcast double* %force_round_fr_mem2081 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %524)
  store volatile double %expt_even_pos_block_res_hi_unroll_x2080, double* %force_round_fr_mem2081, align 8
  %force_roundfr_rounded2082 = load volatile double* %force_round_fr_mem2081, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %524)
  %force_round_fr_mem2084 = alloca double, align 8
  %525 = bitcast double* %force_round_fr_mem2084 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %525)
  store volatile double %force_roundfr_rounded2082, double* %force_round_fr_mem2084, align 8
  %force_roundfr_rounded2085 = load volatile double* %force_round_fr_mem2084, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %525)
  br label %expt_even_merge2128

expt_even_both_are_not_pos2086:                   ; preds = %expt_even_merge2025
  %expt_even_are_both_neg_p2087 = fcmp olt double %mul_hi_abcd_max2047, 0.000000e+00
  br i1 %expt_even_are_both_neg_p2087, label %expt_even_both_are_neg2088, label %expt_even_contains_zero2107

expt_even_both_are_neg2088:                       ; preds = %expt_even_both_are_not_pos2086
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x2089 = fmul double %mul_hi_abcd_max2047, %mul_hi_abcd_max2047
  %force_round_fr_mem2090 = alloca double, align 8
  %526 = bitcast double* %force_round_fr_mem2090 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %526)
  store volatile double %expt_even_neg_block_res_lo_unroll_x2089, double* %force_round_fr_mem2090, align 8
  %force_roundfr_rounded2091 = load volatile double* %force_round_fr_mem2090, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %526)
  %expt_even_neg_block_res_lo_unroll_x2092 = fmul double %force_roundfr_rounded2091, %force_roundfr_rounded2091
  %force_round_fr_mem2093 = alloca double, align 8
  %527 = bitcast double* %force_round_fr_mem2093 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %527)
  store volatile double %expt_even_neg_block_res_lo_unroll_x2092, double* %force_round_fr_mem2093, align 8
  %force_roundfr_rounded2094 = load volatile double* %force_round_fr_mem2093, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %527)
  %force_round_fr_mem2096 = alloca double, align 8
  %528 = bitcast double* %force_round_fr_mem2096 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %528)
  store volatile double %force_roundfr_rounded2094, double* %force_round_fr_mem2096, align 8
  %force_roundfr_rounded2097 = load volatile double* %force_round_fr_mem2096, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %528)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x2098 = fmul double %mul_lo_abcd_min2065, %mul_lo_abcd_min2065
  %force_round_fr_mem2099 = alloca double, align 8
  %529 = bitcast double* %force_round_fr_mem2099 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %529)
  store volatile double %expt_even_pos_block_res_hi_unroll_x2098, double* %force_round_fr_mem2099, align 8
  %force_roundfr_rounded2100 = load volatile double* %force_round_fr_mem2099, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %529)
  %expt_even_pos_block_res_hi_unroll_x2101 = fmul double %force_roundfr_rounded2100, %force_roundfr_rounded2100
  %force_round_fr_mem2102 = alloca double, align 8
  %530 = bitcast double* %force_round_fr_mem2102 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %530)
  store volatile double %expt_even_pos_block_res_hi_unroll_x2101, double* %force_round_fr_mem2102, align 8
  %force_roundfr_rounded2103 = load volatile double* %force_round_fr_mem2102, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %530)
  %force_round_fr_mem2105 = alloca double, align 8
  %531 = bitcast double* %force_round_fr_mem2105 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %531)
  store volatile double %force_roundfr_rounded2103, double* %force_round_fr_mem2105, align 8
  %force_roundfr_rounded2106 = load volatile double* %force_round_fr_mem2105, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %531)
  br label %expt_even_merge2128

expt_even_contains_zero2107:                      ; preds = %expt_even_both_are_not_pos2086
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x2108 = fmul double %mul_lo_abcd_min2065, %mul_lo_abcd_min2065
  %force_round_fr_mem2109 = alloca double, align 8
  %532 = bitcast double* %force_round_fr_mem2109 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %532)
  store volatile double %expt_even_mid_block_lo__unroll_x2108, double* %force_round_fr_mem2109, align 8
  %force_roundfr_rounded2110 = load volatile double* %force_round_fr_mem2109, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %532)
  %expt_even_mid_block_lo__unroll_x2111 = fmul double %force_roundfr_rounded2110, %force_roundfr_rounded2110
  %force_round_fr_mem2112 = alloca double, align 8
  %533 = bitcast double* %force_round_fr_mem2112 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %533)
  store volatile double %expt_even_mid_block_lo__unroll_x2111, double* %force_round_fr_mem2112, align 8
  %force_roundfr_rounded2113 = load volatile double* %force_round_fr_mem2112, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %533)
  %force_round_fr_mem2115 = alloca double, align 8
  %534 = bitcast double* %force_round_fr_mem2115 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %534)
  store volatile double %force_roundfr_rounded2113, double* %force_round_fr_mem2115, align 8
  %force_roundfr_rounded2116 = load volatile double* %force_round_fr_mem2115, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %534)
  %expt_even_mid_block_hi__unroll_x2117 = fmul double %mul_hi_abcd_max2047, %mul_hi_abcd_max2047
  %force_round_fr_mem2118 = alloca double, align 8
  %535 = bitcast double* %force_round_fr_mem2118 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %535)
  store volatile double %expt_even_mid_block_hi__unroll_x2117, double* %force_round_fr_mem2118, align 8
  %force_roundfr_rounded2119 = load volatile double* %force_round_fr_mem2118, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %535)
  %expt_even_mid_block_hi__unroll_x2120 = fmul double %force_roundfr_rounded2119, %force_roundfr_rounded2119
  %force_round_fr_mem2121 = alloca double, align 8
  %536 = bitcast double* %force_round_fr_mem2121 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %536)
  store volatile double %expt_even_mid_block_hi__unroll_x2120, double* %force_round_fr_mem2121, align 8
  %force_roundfr_rounded2122 = load volatile double* %force_round_fr_mem2121, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %536)
  %force_round_fr_mem2124 = alloca double, align 8
  %537 = bitcast double* %force_round_fr_mem2124 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %537)
  store volatile double %force_roundfr_rounded2122, double* %force_round_fr_mem2124, align 8
  %force_roundfr_rounded2125 = load volatile double* %force_round_fr_mem2124, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %537)
  %expt_even_mid_block_res_hi_abtest2126 = fcmp olt double %force_roundfr_rounded2116, %force_roundfr_rounded2125
  %expt_even_mid_block_res_hi_abmax2127 = select i1 %expt_even_mid_block_res_hi_abtest2126, double %force_roundfr_rounded2125, double %force_roundfr_rounded2116
  br label %expt_even_merge2128

expt_even_merge2128:                              ; preds = %expt_even_contains_zero2107, %expt_even_both_are_neg2088, %expt_even_both_are_pos2067
  %expt_lo2129 = phi double [ %force_roundfr_rounded2076, %expt_even_both_are_pos2067 ], [ %force_roundfr_rounded2097, %expt_even_both_are_neg2088 ], [ 0.000000e+00, %expt_even_contains_zero2107 ]
  %expt_hi2130 = phi double [ %force_roundfr_rounded2085, %expt_even_both_are_pos2067 ], [ %force_roundfr_rounded2106, %expt_even_both_are_neg2088 ], [ %expt_even_mid_block_res_hi_abmax2127, %expt_even_contains_zero2107 ]
  call void @set_rounding_mode_down()
  %sub_lo2135 = fsub double %y_lo, %expt_hi2130
  %force_round_fr_mem2136 = alloca double, align 8
  %538 = bitcast double* %force_round_fr_mem2136 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %538)
  store volatile double %sub_lo2135, double* %force_round_fr_mem2136, align 8
  %force_roundfr_rounded2137 = load volatile double* %force_round_fr_mem2136, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %538)
  call void @set_rounding_mode_up()
  %sub_hi2138 = fsub double %y_hi, %expt_lo2129
  %force_round_fr_mem2139 = alloca double, align 8
  %539 = bitcast double* %force_round_fr_mem2139 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %539)
  store volatile double %sub_hi2138, double* %force_round_fr_mem2139, align 8
  %force_roundfr_rounded2140 = load volatile double* %force_round_fr_mem2139, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %539)
  %mul_hi_a2141 = fmul double %force_roundfr_rounded2137, %z_lo
  %force_round_fr_mem2142 = alloca double, align 8
  %540 = bitcast double* %force_round_fr_mem2142 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %540)
  store volatile double %mul_hi_a2141, double* %force_round_fr_mem2142, align 8
  %force_roundfr_rounded2143 = load volatile double* %force_round_fr_mem2142, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %540)
  %mul_hi_b2144 = fmul double %force_roundfr_rounded2140, %z_lo
  %force_round_fr_mem2145 = alloca double, align 8
  %541 = bitcast double* %force_round_fr_mem2145 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %541)
  store volatile double %mul_hi_b2144, double* %force_round_fr_mem2145, align 8
  %force_roundfr_rounded2146 = load volatile double* %force_round_fr_mem2145, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %541)
  %mul_hi_c2147 = fmul double %force_roundfr_rounded2137, %z_hi
  %force_round_fr_mem2148 = alloca double, align 8
  %542 = bitcast double* %force_round_fr_mem2148 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %542)
  store volatile double %mul_hi_c2147, double* %force_round_fr_mem2148, align 8
  %force_roundfr_rounded2149 = load volatile double* %force_round_fr_mem2148, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %542)
  %mul_hi_d2150 = fmul double %force_roundfr_rounded2140, %z_hi
  %force_round_fr_mem2151 = alloca double, align 8
  %543 = bitcast double* %force_round_fr_mem2151 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %543)
  store volatile double %mul_hi_d2150, double* %force_round_fr_mem2151, align 8
  %force_roundfr_rounded2152 = load volatile double* %force_round_fr_mem2151, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %543)
  %mul_hi_ab_test2153 = fcmp olt double %force_roundfr_rounded2143, %force_roundfr_rounded2146
  %mul_hi_cd_test2154 = fcmp olt double %force_roundfr_rounded2149, %force_roundfr_rounded2152
  %mul_hi_ab_max2155 = select i1 %mul_hi_ab_test2153, double %force_roundfr_rounded2146, double %force_roundfr_rounded2143
  %mul_hi_cd_max2156 = select i1 %mul_hi_cd_test2154, double %force_roundfr_rounded2152, double %force_roundfr_rounded2149
  %mul_hi_abcd_test2157 = fcmp olt double %mul_hi_ab_max2155, %mul_hi_cd_max2156
  %mul_hi_abcd_max2158 = select i1 %mul_hi_abcd_test2157, double %mul_hi_cd_max2156, double %mul_hi_ab_max2155
  call void @set_rounding_mode_down()
  %force_round_fr_mem2160 = alloca double, align 8
  %544 = bitcast double* %force_round_fr_mem2160 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %544)
  store volatile double %mul_hi_a2141, double* %force_round_fr_mem2160, align 8
  %force_roundfr_rounded2161 = load volatile double* %force_round_fr_mem2160, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %544)
  %force_round_fr_mem2163 = alloca double, align 8
  %545 = bitcast double* %force_round_fr_mem2163 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %545)
  store volatile double %mul_hi_b2144, double* %force_round_fr_mem2163, align 8
  %force_roundfr_rounded2164 = load volatile double* %force_round_fr_mem2163, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %545)
  %force_round_fr_mem2166 = alloca double, align 8
  %546 = bitcast double* %force_round_fr_mem2166 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %546)
  store volatile double %mul_hi_c2147, double* %force_round_fr_mem2166, align 8
  %force_roundfr_rounded2167 = load volatile double* %force_round_fr_mem2166, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %546)
  %force_round_fr_mem2169 = alloca double, align 8
  %547 = bitcast double* %force_round_fr_mem2169 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %547)
  store volatile double %mul_hi_d2150, double* %force_round_fr_mem2169, align 8
  %force_roundfr_rounded2170 = load volatile double* %force_round_fr_mem2169, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %547)
  %mul_lo_ab_test2171 = fcmp olt double %force_roundfr_rounded2164, %force_roundfr_rounded2161
  %mul_lo_cd_test2172 = fcmp olt double %force_roundfr_rounded2170, %force_roundfr_rounded2167
  %mul_lo_cd_min2173 = select i1 %mul_lo_cd_test2172, double %force_roundfr_rounded2170, double %force_roundfr_rounded2167
  %mul_lo_ab_min2174 = select i1 %mul_lo_ab_test2171, double %force_roundfr_rounded2164, double %force_roundfr_rounded2161
  %mul_lo_abcd_test2175 = fcmp olt double %mul_lo_cd_min2173, %mul_lo_ab_min2174
  %mul_lo_abcd_min2176 = select i1 %mul_lo_abcd_test2175, double %mul_lo_cd_min2173, double %mul_lo_ab_min2174
  %sub_lo2179 = fsub double -4.300000e+01, %mul_hi_abcd_max2158
  %force_round_fr_mem2180 = alloca double, align 8
  %548 = bitcast double* %force_round_fr_mem2180 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %548)
  store volatile double %sub_lo2179, double* %force_round_fr_mem2180, align 8
  %force_roundfr_rounded2181 = load volatile double* %force_round_fr_mem2180, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %548)
  call void @set_rounding_mode_up()
  %sub_hi2182 = fsub double -4.300000e+01, %mul_lo_abcd_min2176
  %force_round_fr_mem2183 = alloca double, align 8
  %549 = bitcast double* %force_round_fr_mem2183 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %549)
  store volatile double %sub_hi2182, double* %force_round_fr_mem2183, align 8
  %force_roundfr_rounded2184 = load volatile double* %force_round_fr_mem2183, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %549)
  %mul_hi_a2185 = fmul double %force_roundfr_rounded2181, %a_lo
  %force_round_fr_mem2186 = alloca double, align 8
  %550 = bitcast double* %force_round_fr_mem2186 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %550)
  store volatile double %mul_hi_a2185, double* %force_round_fr_mem2186, align 8
  %force_roundfr_rounded2187 = load volatile double* %force_round_fr_mem2186, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %550)
  %mul_hi_b2188 = fmul double %force_roundfr_rounded2184, %a_lo
  %force_round_fr_mem2189 = alloca double, align 8
  %551 = bitcast double* %force_round_fr_mem2189 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %551)
  store volatile double %mul_hi_b2188, double* %force_round_fr_mem2189, align 8
  %force_roundfr_rounded2190 = load volatile double* %force_round_fr_mem2189, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %551)
  %mul_hi_c2191 = fmul double %force_roundfr_rounded2181, %a_hi
  %force_round_fr_mem2192 = alloca double, align 8
  %552 = bitcast double* %force_round_fr_mem2192 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %552)
  store volatile double %mul_hi_c2191, double* %force_round_fr_mem2192, align 8
  %force_roundfr_rounded2193 = load volatile double* %force_round_fr_mem2192, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %552)
  %mul_hi_d2194 = fmul double %force_roundfr_rounded2184, %a_hi
  %force_round_fr_mem2195 = alloca double, align 8
  %553 = bitcast double* %force_round_fr_mem2195 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %553)
  store volatile double %mul_hi_d2194, double* %force_round_fr_mem2195, align 8
  %force_roundfr_rounded2196 = load volatile double* %force_round_fr_mem2195, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %553)
  %mul_hi_ab_test2197 = fcmp olt double %force_roundfr_rounded2187, %force_roundfr_rounded2190
  %mul_hi_cd_test2198 = fcmp olt double %force_roundfr_rounded2193, %force_roundfr_rounded2196
  %mul_hi_ab_max2199 = select i1 %mul_hi_ab_test2197, double %force_roundfr_rounded2190, double %force_roundfr_rounded2187
  %mul_hi_cd_max2200 = select i1 %mul_hi_cd_test2198, double %force_roundfr_rounded2196, double %force_roundfr_rounded2193
  %mul_hi_abcd_test2201 = fcmp olt double %mul_hi_ab_max2199, %mul_hi_cd_max2200
  %mul_hi_abcd_max2202 = select i1 %mul_hi_abcd_test2201, double %mul_hi_cd_max2200, double %mul_hi_ab_max2199
  call void @set_rounding_mode_down()
  %force_round_fr_mem2204 = alloca double, align 8
  %554 = bitcast double* %force_round_fr_mem2204 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %554)
  store volatile double %mul_hi_a2185, double* %force_round_fr_mem2204, align 8
  %force_roundfr_rounded2205 = load volatile double* %force_round_fr_mem2204, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %554)
  %force_round_fr_mem2207 = alloca double, align 8
  %555 = bitcast double* %force_round_fr_mem2207 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %555)
  store volatile double %mul_hi_b2188, double* %force_round_fr_mem2207, align 8
  %force_roundfr_rounded2208 = load volatile double* %force_round_fr_mem2207, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %555)
  %force_round_fr_mem2210 = alloca double, align 8
  %556 = bitcast double* %force_round_fr_mem2210 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %556)
  store volatile double %mul_hi_c2191, double* %force_round_fr_mem2210, align 8
  %force_roundfr_rounded2211 = load volatile double* %force_round_fr_mem2210, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %556)
  %force_round_fr_mem2213 = alloca double, align 8
  %557 = bitcast double* %force_round_fr_mem2213 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %557)
  store volatile double %mul_hi_d2194, double* %force_round_fr_mem2213, align 8
  %force_roundfr_rounded2214 = load volatile double* %force_round_fr_mem2213, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %557)
  %mul_lo_ab_test2215 = fcmp olt double %force_roundfr_rounded2208, %force_roundfr_rounded2205
  %mul_lo_cd_test2216 = fcmp olt double %force_roundfr_rounded2214, %force_roundfr_rounded2211
  %mul_lo_cd_min2217 = select i1 %mul_lo_cd_test2216, double %force_roundfr_rounded2214, double %force_roundfr_rounded2211
  %mul_lo_ab_min2218 = select i1 %mul_lo_ab_test2215, double %force_roundfr_rounded2208, double %force_roundfr_rounded2205
  %mul_lo_abcd_test2219 = fcmp olt double %mul_lo_cd_min2217, %mul_lo_ab_min2218
  %mul_lo_abcd_min2220 = select i1 %mul_lo_abcd_test2219, double %mul_lo_cd_min2217, double %mul_lo_ab_min2218
  %add_lo2221 = fadd double %mul_lo_abcd_min2220, %e_lo
  %force_round_fr_mem2222 = alloca double, align 8
  %558 = bitcast double* %force_round_fr_mem2222 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %558)
  store volatile double %add_lo2221, double* %force_round_fr_mem2222, align 8
  %force_roundfr_rounded2223 = load volatile double* %force_round_fr_mem2222, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %558)
  call void @set_rounding_mode_up()
  %add_hi2224 = fadd double %mul_hi_abcd_max2202, %e_hi
  %force_round_fr_mem2225 = alloca double, align 8
  %559 = bitcast double* %force_round_fr_mem2225 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %559)
  store volatile double %add_hi2224, double* %force_round_fr_mem2225, align 8
  %force_roundfr_rounded2226 = load volatile double* %force_round_fr_mem2225, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %559)
  call void @set_rounding_mode_down()
  %sub_lo2227 = fsub double %g_lo, %force_roundfr_rounded2226
  %force_round_fr_mem2228 = alloca double, align 8
  %560 = bitcast double* %force_round_fr_mem2228 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %560)
  store volatile double %sub_lo2227, double* %force_round_fr_mem2228, align 8
  %force_roundfr_rounded2229 = load volatile double* %force_round_fr_mem2228, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %560)
  call void @set_rounding_mode_up()
  %sub_hi2230 = fsub double %g_hi, %force_roundfr_rounded2223
  %force_round_fr_mem2231 = alloca double, align 8
  %561 = bitcast double* %force_round_fr_mem2231 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %561)
  store volatile double %sub_hi2230, double* %force_round_fr_mem2231, align 8
  %force_roundfr_rounded2232 = load volatile double* %force_round_fr_mem2231, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %561)
  call void @set_rounding_mode_down()
  %sub_lo2233 = fsub double -6.800000e+01, %force_roundfr_rounded2232
  %force_round_fr_mem2234 = alloca double, align 8
  %562 = bitcast double* %force_round_fr_mem2234 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %562)
  store volatile double %sub_lo2233, double* %force_round_fr_mem2234, align 8
  %force_roundfr_rounded2235 = load volatile double* %force_round_fr_mem2234, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %562)
  call void @set_rounding_mode_up()
  %sub_hi2236 = fsub double -6.800000e+01, %force_roundfr_rounded2229
  %force_round_fr_mem2237 = alloca double, align 8
  %563 = bitcast double* %force_round_fr_mem2237 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %563)
  store volatile double %sub_hi2236, double* %force_round_fr_mem2237, align 8
  %force_roundfr_rounded2238 = load volatile double* %force_round_fr_mem2237, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %563)
  call void @set_rounding_mode_down()
  %sub_lo2239 = fsub double %h_lo, %force_roundfr_rounded2238
  %force_round_fr_mem2240 = alloca double, align 8
  %564 = bitcast double* %force_round_fr_mem2240 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %564)
  store volatile double %sub_lo2239, double* %force_round_fr_mem2240, align 8
  %force_roundfr_rounded2241 = load volatile double* %force_round_fr_mem2240, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %564)
  call void @set_rounding_mode_up()
  %sub_hi2242 = fsub double %h_hi, %force_roundfr_rounded2235
  %force_round_fr_mem2243 = alloca double, align 8
  %565 = bitcast double* %force_round_fr_mem2243 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %565)
  store volatile double %sub_hi2242, double* %force_round_fr_mem2243, align 8
  %force_roundfr_rounded2244 = load volatile double* %force_round_fr_mem2243, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %565)
  call void @set_rounding_mode_down()
  call void @set_rounding_mode_up()
  call void @set_rounding_mode_down()
  %add_lo2245 = fadd double %force_roundfr_rounded2241, %aa_lo
  %force_round_fr_mem2246 = alloca double, align 8
  %566 = bitcast double* %force_round_fr_mem2246 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %566)
  store volatile double %add_lo2245, double* %force_round_fr_mem2246, align 8
  %force_roundfr_rounded2247 = load volatile double* %force_round_fr_mem2246, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %566)
  call void @set_rounding_mode_up()
  %add_hi2248 = fadd double %force_roundfr_rounded2244, %aa_hi
  %force_round_fr_mem2249 = alloca double, align 8
  %567 = bitcast double* %force_round_fr_mem2249 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %567)
  store volatile double %add_hi2248, double* %force_round_fr_mem2249, align 8
  %force_roundfr_rounded2250 = load volatile double* %force_round_fr_mem2249, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %567)
  call void @set_rounding_mode_down()
  %add_lo2251 = fadd double %force_roundfr_rounded2247, %zy_lo
  %force_round_fr_mem2252 = alloca double, align 8
  %568 = bitcast double* %force_round_fr_mem2252 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %568)
  store volatile double %add_lo2251, double* %force_round_fr_mem2252, align 8
  %force_roundfr_rounded2253 = load volatile double* %force_round_fr_mem2252, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %568)
  call void @set_rounding_mode_up()
  %add_hi2254 = fadd double %force_roundfr_rounded2250, %zy_hi
  %force_round_fr_mem2255 = alloca double, align 8
  %569 = bitcast double* %force_round_fr_mem2255 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %569)
  store volatile double %add_hi2254, double* %force_round_fr_mem2255, align 8
  %force_roundfr_rounded2256 = load volatile double* %force_round_fr_mem2255, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %569)
  %div_hi_a2257 = fdiv double %force_roundfr_rounded2253, %zx_lo
  %force_round_fr_mem2258 = alloca double, align 8
  %570 = bitcast double* %force_round_fr_mem2258 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %570)
  store volatile double %div_hi_a2257, double* %force_round_fr_mem2258, align 8
  %force_roundfr_rounded2259 = load volatile double* %force_round_fr_mem2258, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %570)
  %div_hi_b2260 = fdiv double %force_roundfr_rounded2253, %zx_hi
  %force_round_fr_mem2261 = alloca double, align 8
  %571 = bitcast double* %force_round_fr_mem2261 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %571)
  store volatile double %div_hi_b2260, double* %force_round_fr_mem2261, align 8
  %force_roundfr_rounded2262 = load volatile double* %force_round_fr_mem2261, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %571)
  %div_hi_c2263 = fdiv double %force_roundfr_rounded2256, %zx_lo
  %force_round_fr_mem2264 = alloca double, align 8
  %572 = bitcast double* %force_round_fr_mem2264 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %572)
  store volatile double %div_hi_c2263, double* %force_round_fr_mem2264, align 8
  %force_roundfr_rounded2265 = load volatile double* %force_round_fr_mem2264, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %572)
  %div_hi_d2266 = fdiv double %force_roundfr_rounded2256, %zx_hi
  %force_round_fr_mem2267 = alloca double, align 8
  %573 = bitcast double* %force_round_fr_mem2267 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %573)
  store volatile double %div_hi_d2266, double* %force_round_fr_mem2267, align 8
  %force_roundfr_rounded2268 = load volatile double* %force_round_fr_mem2267, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %573)
  %div_hi_ab_test2269 = fcmp olt double %force_roundfr_rounded2259, %force_roundfr_rounded2262
  %div_hi_cd_test2270 = fcmp olt double %force_roundfr_rounded2265, %force_roundfr_rounded2268
  %div_hi_ab_max2271 = select i1 %div_hi_ab_test2269, double %force_roundfr_rounded2262, double %force_roundfr_rounded2259
  %div_hi_cd_max2272 = select i1 %div_hi_cd_test2270, double %force_roundfr_rounded2268, double %force_roundfr_rounded2265
  %div_hi_abcd_test2273 = fcmp olt double %div_hi_ab_max2271, %div_hi_cd_max2272
  %div_hi_abcd_max2274 = select i1 %div_hi_abcd_test2273, double %div_hi_cd_max2272, double %div_hi_ab_max2271
  call void @set_rounding_mode_down()
  %force_round_fr_mem2276 = alloca double, align 8
  %574 = bitcast double* %force_round_fr_mem2276 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %574)
  store volatile double %div_hi_a2257, double* %force_round_fr_mem2276, align 8
  %force_roundfr_rounded2277 = load volatile double* %force_round_fr_mem2276, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %574)
  %force_round_fr_mem2279 = alloca double, align 8
  %575 = bitcast double* %force_round_fr_mem2279 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %575)
  store volatile double %div_hi_b2260, double* %force_round_fr_mem2279, align 8
  %force_roundfr_rounded2280 = load volatile double* %force_round_fr_mem2279, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %575)
  %force_round_fr_mem2282 = alloca double, align 8
  %576 = bitcast double* %force_round_fr_mem2282 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %576)
  store volatile double %div_hi_c2263, double* %force_round_fr_mem2282, align 8
  %force_roundfr_rounded2283 = load volatile double* %force_round_fr_mem2282, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %576)
  %force_round_fr_mem2285 = alloca double, align 8
  %577 = bitcast double* %force_round_fr_mem2285 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %577)
  store volatile double %div_hi_d2266, double* %force_round_fr_mem2285, align 8
  %force_roundfr_rounded2286 = load volatile double* %force_round_fr_mem2285, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %577)
  %div_lo_ab_test2287 = fcmp olt double %force_roundfr_rounded2280, %force_roundfr_rounded2277
  %div_lo_cd_test2288 = fcmp olt double %force_roundfr_rounded2286, %force_roundfr_rounded2283
  %div_lo_cd_min2289 = select i1 %div_lo_cd_test2288, double %force_roundfr_rounded2286, double %force_roundfr_rounded2283
  %div_lo_ab_min2290 = select i1 %div_lo_ab_test2287, double %force_roundfr_rounded2280, double %force_roundfr_rounded2277
  %div_lo_abcd_test2291 = fcmp olt double %div_lo_cd_min2289, %div_lo_ab_min2290
  %div_lo_abcd_min2292 = select i1 %div_lo_abcd_test2291, double %div_lo_cd_min2289, double %div_lo_ab_min2290
  %add_lo2293 = fadd double %div_lo_abcd_min2292, %zw_lo
  %force_round_fr_mem2294 = alloca double, align 8
  %578 = bitcast double* %force_round_fr_mem2294 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %578)
  store volatile double %add_lo2293, double* %force_round_fr_mem2294, align 8
  %force_roundfr_rounded2295 = load volatile double* %force_round_fr_mem2294, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %578)
  call void @set_rounding_mode_up()
  %add_hi2296 = fadd double %div_hi_abcd_max2274, %zw_hi
  %force_round_fr_mem2297 = alloca double, align 8
  %579 = bitcast double* %force_round_fr_mem2297 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %579)
  store volatile double %add_hi2296, double* %force_round_fr_mem2297, align 8
  %force_roundfr_rounded2298 = load volatile double* %force_round_fr_mem2297, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %579)
  call void @set_rounding_mode_down()
  %add_lo2299 = fadd double %force_roundfr_rounded2295, %zv_lo
  %force_round_fr_mem2300 = alloca double, align 8
  %580 = bitcast double* %force_round_fr_mem2300 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %580)
  store volatile double %add_lo2299, double* %force_round_fr_mem2300, align 8
  %force_roundfr_rounded2301 = load volatile double* %force_round_fr_mem2300, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %580)
  call void @set_rounding_mode_up()
  %add_hi2302 = fadd double %force_roundfr_rounded2298, %zv_hi
  %force_round_fr_mem2303 = alloca double, align 8
  %581 = bitcast double* %force_round_fr_mem2303 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %581)
  store volatile double %add_hi2302, double* %force_round_fr_mem2303, align 8
  %force_roundfr_rounded2304 = load volatile double* %force_round_fr_mem2303, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %581)
  call void @set_rounding_mode_down()
  %add_lo2305 = fadd double %force_roundfr_rounded2301, %zu_lo
  %force_round_fr_mem2306 = alloca double, align 8
  %582 = bitcast double* %force_round_fr_mem2306 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %582)
  store volatile double %add_lo2305, double* %force_round_fr_mem2306, align 8
  %force_roundfr_rounded2307 = load volatile double* %force_round_fr_mem2306, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %582)
  call void @set_rounding_mode_up()
  %add_hi2308 = fadd double %force_roundfr_rounded2304, %zu_hi
  %force_round_fr_mem2309 = alloca double, align 8
  %583 = bitcast double* %force_round_fr_mem2309 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %583)
  store volatile double %add_hi2308, double* %force_round_fr_mem2309, align 8
  %force_roundfr_rounded2310 = load volatile double* %force_round_fr_mem2309, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %583)
  %mul_hi_a2311 = fmul double %force_roundfr_rounded2307, %zt_lo
  %force_round_fr_mem2312 = alloca double, align 8
  %584 = bitcast double* %force_round_fr_mem2312 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %584)
  store volatile double %mul_hi_a2311, double* %force_round_fr_mem2312, align 8
  %force_roundfr_rounded2313 = load volatile double* %force_round_fr_mem2312, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %584)
  %mul_hi_b2314 = fmul double %force_roundfr_rounded2310, %zt_lo
  %force_round_fr_mem2315 = alloca double, align 8
  %585 = bitcast double* %force_round_fr_mem2315 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %585)
  store volatile double %mul_hi_b2314, double* %force_round_fr_mem2315, align 8
  %force_roundfr_rounded2316 = load volatile double* %force_round_fr_mem2315, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %585)
  %mul_hi_c2317 = fmul double %force_roundfr_rounded2307, %zt_hi
  %force_round_fr_mem2318 = alloca double, align 8
  %586 = bitcast double* %force_round_fr_mem2318 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %586)
  store volatile double %mul_hi_c2317, double* %force_round_fr_mem2318, align 8
  %force_roundfr_rounded2319 = load volatile double* %force_round_fr_mem2318, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %586)
  %mul_hi_d2320 = fmul double %force_roundfr_rounded2310, %zt_hi
  %force_round_fr_mem2321 = alloca double, align 8
  %587 = bitcast double* %force_round_fr_mem2321 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %587)
  store volatile double %mul_hi_d2320, double* %force_round_fr_mem2321, align 8
  %force_roundfr_rounded2322 = load volatile double* %force_round_fr_mem2321, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %587)
  %mul_hi_ab_test2323 = fcmp olt double %force_roundfr_rounded2313, %force_roundfr_rounded2316
  %mul_hi_cd_test2324 = fcmp olt double %force_roundfr_rounded2319, %force_roundfr_rounded2322
  %mul_hi_ab_max2325 = select i1 %mul_hi_ab_test2323, double %force_roundfr_rounded2316, double %force_roundfr_rounded2313
  %mul_hi_cd_max2326 = select i1 %mul_hi_cd_test2324, double %force_roundfr_rounded2322, double %force_roundfr_rounded2319
  %mul_hi_abcd_test2327 = fcmp olt double %mul_hi_ab_max2325, %mul_hi_cd_max2326
  %mul_hi_abcd_max2328 = select i1 %mul_hi_abcd_test2327, double %mul_hi_cd_max2326, double %mul_hi_ab_max2325
  call void @set_rounding_mode_down()
  %force_round_fr_mem2330 = alloca double, align 8
  %588 = bitcast double* %force_round_fr_mem2330 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %588)
  store volatile double %mul_hi_a2311, double* %force_round_fr_mem2330, align 8
  %force_roundfr_rounded2331 = load volatile double* %force_round_fr_mem2330, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %588)
  %force_round_fr_mem2333 = alloca double, align 8
  %589 = bitcast double* %force_round_fr_mem2333 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %589)
  store volatile double %mul_hi_b2314, double* %force_round_fr_mem2333, align 8
  %force_roundfr_rounded2334 = load volatile double* %force_round_fr_mem2333, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %589)
  %force_round_fr_mem2336 = alloca double, align 8
  %590 = bitcast double* %force_round_fr_mem2336 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %590)
  store volatile double %mul_hi_c2317, double* %force_round_fr_mem2336, align 8
  %force_roundfr_rounded2337 = load volatile double* %force_round_fr_mem2336, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %590)
  %force_round_fr_mem2339 = alloca double, align 8
  %591 = bitcast double* %force_round_fr_mem2339 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %591)
  store volatile double %mul_hi_d2320, double* %force_round_fr_mem2339, align 8
  %force_roundfr_rounded2340 = load volatile double* %force_round_fr_mem2339, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %591)
  %mul_lo_ab_test2341 = fcmp olt double %force_roundfr_rounded2334, %force_roundfr_rounded2331
  %mul_lo_cd_test2342 = fcmp olt double %force_roundfr_rounded2340, %force_roundfr_rounded2337
  %mul_lo_cd_min2343 = select i1 %mul_lo_cd_test2342, double %force_roundfr_rounded2340, double %force_roundfr_rounded2337
  %mul_lo_ab_min2344 = select i1 %mul_lo_ab_test2341, double %force_roundfr_rounded2334, double %force_roundfr_rounded2331
  %mul_lo_abcd_test2345 = fcmp olt double %mul_lo_cd_min2343, %mul_lo_ab_min2344
  %mul_lo_abcd_min2346 = select i1 %mul_lo_abcd_test2345, double %mul_lo_cd_min2343, double %mul_lo_ab_min2344
  %sub_lo2347 = fsub double %mul_lo_abcd_min2346, %zs_hi
  %force_round_fr_mem2348 = alloca double, align 8
  %592 = bitcast double* %force_round_fr_mem2348 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %592)
  store volatile double %sub_lo2347, double* %force_round_fr_mem2348, align 8
  %force_roundfr_rounded2349 = load volatile double* %force_round_fr_mem2348, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %592)
  call void @set_rounding_mode_up()
  %sub_hi2350 = fsub double %mul_hi_abcd_max2328, %zs_lo
  %force_round_fr_mem2351 = alloca double, align 8
  %593 = bitcast double* %force_round_fr_mem2351 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %593)
  store volatile double %sub_hi2350, double* %force_round_fr_mem2351, align 8
  %force_roundfr_rounded2352 = load volatile double* %force_round_fr_mem2351, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %593)
  %div_hi_a2353 = fdiv double %force_roundfr_rounded2349, %zr_lo
  %force_round_fr_mem2354 = alloca double, align 8
  %594 = bitcast double* %force_round_fr_mem2354 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %594)
  store volatile double %div_hi_a2353, double* %force_round_fr_mem2354, align 8
  %force_roundfr_rounded2355 = load volatile double* %force_round_fr_mem2354, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %594)
  %div_hi_b2356 = fdiv double %force_roundfr_rounded2349, %zr_hi
  %force_round_fr_mem2357 = alloca double, align 8
  %595 = bitcast double* %force_round_fr_mem2357 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %595)
  store volatile double %div_hi_b2356, double* %force_round_fr_mem2357, align 8
  %force_roundfr_rounded2358 = load volatile double* %force_round_fr_mem2357, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %595)
  %div_hi_c2359 = fdiv double %force_roundfr_rounded2352, %zr_lo
  %force_round_fr_mem2360 = alloca double, align 8
  %596 = bitcast double* %force_round_fr_mem2360 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %596)
  store volatile double %div_hi_c2359, double* %force_round_fr_mem2360, align 8
  %force_roundfr_rounded2361 = load volatile double* %force_round_fr_mem2360, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %596)
  %div_hi_d2362 = fdiv double %force_roundfr_rounded2352, %zr_hi
  %force_round_fr_mem2363 = alloca double, align 8
  %597 = bitcast double* %force_round_fr_mem2363 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %597)
  store volatile double %div_hi_d2362, double* %force_round_fr_mem2363, align 8
  %force_roundfr_rounded2364 = load volatile double* %force_round_fr_mem2363, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %597)
  %div_hi_ab_test2365 = fcmp olt double %force_roundfr_rounded2355, %force_roundfr_rounded2358
  %div_hi_cd_test2366 = fcmp olt double %force_roundfr_rounded2361, %force_roundfr_rounded2364
  %div_hi_ab_max2367 = select i1 %div_hi_ab_test2365, double %force_roundfr_rounded2358, double %force_roundfr_rounded2355
  %div_hi_cd_max2368 = select i1 %div_hi_cd_test2366, double %force_roundfr_rounded2364, double %force_roundfr_rounded2361
  %div_hi_abcd_test2369 = fcmp olt double %div_hi_ab_max2367, %div_hi_cd_max2368
  %div_hi_abcd_max2370 = select i1 %div_hi_abcd_test2369, double %div_hi_cd_max2368, double %div_hi_ab_max2367
  call void @set_rounding_mode_down()
  %force_round_fr_mem2372 = alloca double, align 8
  %598 = bitcast double* %force_round_fr_mem2372 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %598)
  store volatile double %div_hi_a2353, double* %force_round_fr_mem2372, align 8
  %force_roundfr_rounded2373 = load volatile double* %force_round_fr_mem2372, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %598)
  %force_round_fr_mem2375 = alloca double, align 8
  %599 = bitcast double* %force_round_fr_mem2375 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %599)
  store volatile double %div_hi_b2356, double* %force_round_fr_mem2375, align 8
  %force_roundfr_rounded2376 = load volatile double* %force_round_fr_mem2375, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %599)
  %force_round_fr_mem2378 = alloca double, align 8
  %600 = bitcast double* %force_round_fr_mem2378 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %600)
  store volatile double %div_hi_c2359, double* %force_round_fr_mem2378, align 8
  %force_roundfr_rounded2379 = load volatile double* %force_round_fr_mem2378, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %600)
  %force_round_fr_mem2381 = alloca double, align 8
  %601 = bitcast double* %force_round_fr_mem2381 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %601)
  store volatile double %div_hi_d2362, double* %force_round_fr_mem2381, align 8
  %force_roundfr_rounded2382 = load volatile double* %force_round_fr_mem2381, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %601)
  %div_lo_ab_test2383 = fcmp olt double %force_roundfr_rounded2376, %force_roundfr_rounded2373
  %div_lo_cd_test2384 = fcmp olt double %force_roundfr_rounded2382, %force_roundfr_rounded2379
  %div_lo_cd_min2385 = select i1 %div_lo_cd_test2384, double %force_roundfr_rounded2382, double %force_roundfr_rounded2379
  %div_lo_ab_min2386 = select i1 %div_lo_ab_test2383, double %force_roundfr_rounded2376, double %force_roundfr_rounded2373
  %div_lo_abcd_test2387 = fcmp olt double %div_lo_cd_min2385, %div_lo_ab_min2386
  %div_lo_abcd_min2388 = select i1 %div_lo_abcd_test2387, double %div_lo_cd_min2385, double %div_lo_ab_min2386
  call void @set_rounding_mode_up()
  %div_hi_a2389 = fdiv double %zq_lo, %div_lo_abcd_min2388
  %force_round_fr_mem2390 = alloca double, align 8
  %602 = bitcast double* %force_round_fr_mem2390 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %602)
  store volatile double %div_hi_a2389, double* %force_round_fr_mem2390, align 8
  %force_roundfr_rounded2391 = load volatile double* %force_round_fr_mem2390, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %602)
  %div_hi_b2392 = fdiv double %zq_lo, %div_hi_abcd_max2370
  %force_round_fr_mem2393 = alloca double, align 8
  %603 = bitcast double* %force_round_fr_mem2393 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %603)
  store volatile double %div_hi_b2392, double* %force_round_fr_mem2393, align 8
  %force_roundfr_rounded2394 = load volatile double* %force_round_fr_mem2393, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %603)
  %div_hi_c2395 = fdiv double %zq_hi, %div_lo_abcd_min2388
  %force_round_fr_mem2396 = alloca double, align 8
  %604 = bitcast double* %force_round_fr_mem2396 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %604)
  store volatile double %div_hi_c2395, double* %force_round_fr_mem2396, align 8
  %force_roundfr_rounded2397 = load volatile double* %force_round_fr_mem2396, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %604)
  %div_hi_d2398 = fdiv double %zq_hi, %div_hi_abcd_max2370
  %force_round_fr_mem2399 = alloca double, align 8
  %605 = bitcast double* %force_round_fr_mem2399 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %605)
  store volatile double %div_hi_d2398, double* %force_round_fr_mem2399, align 8
  %force_roundfr_rounded2400 = load volatile double* %force_round_fr_mem2399, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %605)
  %div_hi_ab_test2401 = fcmp olt double %force_roundfr_rounded2391, %force_roundfr_rounded2394
  %div_hi_cd_test2402 = fcmp olt double %force_roundfr_rounded2397, %force_roundfr_rounded2400
  %div_hi_ab_max2403 = select i1 %div_hi_ab_test2401, double %force_roundfr_rounded2394, double %force_roundfr_rounded2391
  %div_hi_cd_max2404 = select i1 %div_hi_cd_test2402, double %force_roundfr_rounded2400, double %force_roundfr_rounded2397
  %div_hi_abcd_test2405 = fcmp olt double %div_hi_ab_max2403, %div_hi_cd_max2404
  %div_hi_abcd_max2406 = select i1 %div_hi_abcd_test2405, double %div_hi_cd_max2404, double %div_hi_ab_max2403
  call void @set_rounding_mode_down()
  %force_round_fr_mem2408 = alloca double, align 8
  %606 = bitcast double* %force_round_fr_mem2408 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %606)
  store volatile double %div_hi_a2389, double* %force_round_fr_mem2408, align 8
  %force_roundfr_rounded2409 = load volatile double* %force_round_fr_mem2408, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %606)
  %force_round_fr_mem2411 = alloca double, align 8
  %607 = bitcast double* %force_round_fr_mem2411 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %607)
  store volatile double %div_hi_b2392, double* %force_round_fr_mem2411, align 8
  %force_roundfr_rounded2412 = load volatile double* %force_round_fr_mem2411, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %607)
  %force_round_fr_mem2414 = alloca double, align 8
  %608 = bitcast double* %force_round_fr_mem2414 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %608)
  store volatile double %div_hi_c2395, double* %force_round_fr_mem2414, align 8
  %force_roundfr_rounded2415 = load volatile double* %force_round_fr_mem2414, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %608)
  %force_round_fr_mem2417 = alloca double, align 8
  %609 = bitcast double* %force_round_fr_mem2417 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %609)
  store volatile double %div_hi_d2398, double* %force_round_fr_mem2417, align 8
  %force_roundfr_rounded2418 = load volatile double* %force_round_fr_mem2417, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %609)
  %div_lo_ab_test2419 = fcmp olt double %force_roundfr_rounded2412, %force_roundfr_rounded2409
  %div_lo_cd_test2420 = fcmp olt double %force_roundfr_rounded2418, %force_roundfr_rounded2415
  %div_lo_cd_min2421 = select i1 %div_lo_cd_test2420, double %force_roundfr_rounded2418, double %force_roundfr_rounded2415
  %div_lo_ab_min2422 = select i1 %div_lo_ab_test2419, double %force_roundfr_rounded2412, double %force_roundfr_rounded2409
  %div_lo_abcd_test2423 = fcmp olt double %div_lo_cd_min2421, %div_lo_ab_min2422
  %div_lo_abcd_min2424 = select i1 %div_lo_abcd_test2423, double %div_lo_cd_min2421, double %div_lo_ab_min2422
  %sub_lo2425 = fsub double %zp_lo, %div_hi_abcd_max2406
  %force_round_fr_mem2426 = alloca double, align 8
  %610 = bitcast double* %force_round_fr_mem2426 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %610)
  store volatile double %sub_lo2425, double* %force_round_fr_mem2426, align 8
  %force_roundfr_rounded2427 = load volatile double* %force_round_fr_mem2426, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %610)
  call void @set_rounding_mode_up()
  %sub_hi2428 = fsub double %zp_hi, %div_lo_abcd_min2424
  %force_round_fr_mem2429 = alloca double, align 8
  %611 = bitcast double* %force_round_fr_mem2429 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %611)
  store volatile double %sub_hi2428, double* %force_round_fr_mem2429, align 8
  %force_roundfr_rounded2430 = load volatile double* %force_round_fr_mem2429, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %611)
  call void @set_rounding_mode_down()
  %add_lo2431 = fadd double %force_roundfr_rounded2427, %zo_lo
  %force_round_fr_mem2432 = alloca double, align 8
  %612 = bitcast double* %force_round_fr_mem2432 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %612)
  store volatile double %add_lo2431, double* %force_round_fr_mem2432, align 8
  %force_roundfr_rounded2433 = load volatile double* %force_round_fr_mem2432, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %612)
  call void @set_rounding_mode_up()
  %add_hi2434 = fadd double %force_roundfr_rounded2430, %zo_hi
  %force_round_fr_mem2435 = alloca double, align 8
  %613 = bitcast double* %force_round_fr_mem2435 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %613)
  store volatile double %add_hi2434, double* %force_round_fr_mem2435, align 8
  %force_roundfr_rounded2436 = load volatile double* %force_round_fr_mem2435, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %613)
  call void @set_rounding_mode_down()
  %sub_lo2437 = fsub double %force_roundfr_rounded2433, %zn_hi
  %force_round_fr_mem2438 = alloca double, align 8
  %614 = bitcast double* %force_round_fr_mem2438 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %614)
  store volatile double %sub_lo2437, double* %force_round_fr_mem2438, align 8
  %force_roundfr_rounded2439 = load volatile double* %force_round_fr_mem2438, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %614)
  call void @set_rounding_mode_up()
  %sub_hi2440 = fsub double %force_roundfr_rounded2436, %zn_lo
  %force_round_fr_mem2441 = alloca double, align 8
  %615 = bitcast double* %force_round_fr_mem2441 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %615)
  store volatile double %sub_hi2440, double* %force_round_fr_mem2441, align 8
  %force_roundfr_rounded2442 = load volatile double* %force_round_fr_mem2441, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %615)
  call void @set_rounding_mode_down()
  %sub_lo2443 = fsub double %zm_lo, %force_roundfr_rounded2442
  %force_round_fr_mem2444 = alloca double, align 8
  %616 = bitcast double* %force_round_fr_mem2444 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %616)
  store volatile double %sub_lo2443, double* %force_round_fr_mem2444, align 8
  %force_roundfr_rounded2445 = load volatile double* %force_round_fr_mem2444, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %616)
  call void @set_rounding_mode_up()
  %sub_hi2446 = fsub double %zm_hi, %force_roundfr_rounded2439
  %force_round_fr_mem2447 = alloca double, align 8
  %617 = bitcast double* %force_round_fr_mem2447 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %617)
  store volatile double %sub_hi2446, double* %force_round_fr_mem2447, align 8
  %force_roundfr_rounded2448 = load volatile double* %force_round_fr_mem2447, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %617)
  %mul_hi_a2449 = fmul double %force_roundfr_rounded2445, %zl_lo
  %force_round_fr_mem2450 = alloca double, align 8
  %618 = bitcast double* %force_round_fr_mem2450 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %618)
  store volatile double %mul_hi_a2449, double* %force_round_fr_mem2450, align 8
  %force_roundfr_rounded2451 = load volatile double* %force_round_fr_mem2450, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %618)
  %mul_hi_b2452 = fmul double %force_roundfr_rounded2448, %zl_lo
  %force_round_fr_mem2453 = alloca double, align 8
  %619 = bitcast double* %force_round_fr_mem2453 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %619)
  store volatile double %mul_hi_b2452, double* %force_round_fr_mem2453, align 8
  %force_roundfr_rounded2454 = load volatile double* %force_round_fr_mem2453, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %619)
  %mul_hi_c2455 = fmul double %force_roundfr_rounded2445, %zl_hi
  %force_round_fr_mem2456 = alloca double, align 8
  %620 = bitcast double* %force_round_fr_mem2456 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %620)
  store volatile double %mul_hi_c2455, double* %force_round_fr_mem2456, align 8
  %force_roundfr_rounded2457 = load volatile double* %force_round_fr_mem2456, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %620)
  %mul_hi_d2458 = fmul double %force_roundfr_rounded2448, %zl_hi
  %force_round_fr_mem2459 = alloca double, align 8
  %621 = bitcast double* %force_round_fr_mem2459 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %621)
  store volatile double %mul_hi_d2458, double* %force_round_fr_mem2459, align 8
  %force_roundfr_rounded2460 = load volatile double* %force_round_fr_mem2459, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %621)
  %mul_hi_ab_test2461 = fcmp olt double %force_roundfr_rounded2451, %force_roundfr_rounded2454
  %mul_hi_cd_test2462 = fcmp olt double %force_roundfr_rounded2457, %force_roundfr_rounded2460
  %mul_hi_ab_max2463 = select i1 %mul_hi_ab_test2461, double %force_roundfr_rounded2454, double %force_roundfr_rounded2451
  %mul_hi_cd_max2464 = select i1 %mul_hi_cd_test2462, double %force_roundfr_rounded2460, double %force_roundfr_rounded2457
  %mul_hi_abcd_test2465 = fcmp olt double %mul_hi_ab_max2463, %mul_hi_cd_max2464
  %mul_hi_abcd_max2466 = select i1 %mul_hi_abcd_test2465, double %mul_hi_cd_max2464, double %mul_hi_ab_max2463
  call void @set_rounding_mode_down()
  %force_round_fr_mem2468 = alloca double, align 8
  %622 = bitcast double* %force_round_fr_mem2468 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %622)
  store volatile double %mul_hi_a2449, double* %force_round_fr_mem2468, align 8
  %force_roundfr_rounded2469 = load volatile double* %force_round_fr_mem2468, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %622)
  %force_round_fr_mem2471 = alloca double, align 8
  %623 = bitcast double* %force_round_fr_mem2471 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %623)
  store volatile double %mul_hi_b2452, double* %force_round_fr_mem2471, align 8
  %force_roundfr_rounded2472 = load volatile double* %force_round_fr_mem2471, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %623)
  %force_round_fr_mem2474 = alloca double, align 8
  %624 = bitcast double* %force_round_fr_mem2474 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %624)
  store volatile double %mul_hi_c2455, double* %force_round_fr_mem2474, align 8
  %force_roundfr_rounded2475 = load volatile double* %force_round_fr_mem2474, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %624)
  %force_round_fr_mem2477 = alloca double, align 8
  %625 = bitcast double* %force_round_fr_mem2477 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %625)
  store volatile double %mul_hi_d2458, double* %force_round_fr_mem2477, align 8
  %force_roundfr_rounded2478 = load volatile double* %force_round_fr_mem2477, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %625)
  %mul_lo_ab_test2479 = fcmp olt double %force_roundfr_rounded2472, %force_roundfr_rounded2469
  %mul_lo_cd_test2480 = fcmp olt double %force_roundfr_rounded2478, %force_roundfr_rounded2475
  %mul_lo_cd_min2481 = select i1 %mul_lo_cd_test2480, double %force_roundfr_rounded2478, double %force_roundfr_rounded2475
  %mul_lo_ab_min2482 = select i1 %mul_lo_ab_test2479, double %force_roundfr_rounded2472, double %force_roundfr_rounded2469
  %mul_lo_abcd_test2483 = fcmp olt double %mul_lo_cd_min2481, %mul_lo_ab_min2482
  %mul_lo_abcd_min2484 = select i1 %mul_lo_abcd_test2483, double %mul_lo_cd_min2481, double %mul_lo_ab_min2482
  call void @set_rounding_mode_up()
  %div_hi_a2485 = fdiv double %mul_lo_abcd_min2484, %zk_lo
  %force_round_fr_mem2486 = alloca double, align 8
  %626 = bitcast double* %force_round_fr_mem2486 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %626)
  store volatile double %div_hi_a2485, double* %force_round_fr_mem2486, align 8
  %force_roundfr_rounded2487 = load volatile double* %force_round_fr_mem2486, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %626)
  %div_hi_b2488 = fdiv double %mul_lo_abcd_min2484, %zk_hi
  %force_round_fr_mem2489 = alloca double, align 8
  %627 = bitcast double* %force_round_fr_mem2489 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %627)
  store volatile double %div_hi_b2488, double* %force_round_fr_mem2489, align 8
  %force_roundfr_rounded2490 = load volatile double* %force_round_fr_mem2489, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %627)
  %div_hi_c2491 = fdiv double %mul_hi_abcd_max2466, %zk_lo
  %force_round_fr_mem2492 = alloca double, align 8
  %628 = bitcast double* %force_round_fr_mem2492 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %628)
  store volatile double %div_hi_c2491, double* %force_round_fr_mem2492, align 8
  %force_roundfr_rounded2493 = load volatile double* %force_round_fr_mem2492, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %628)
  %div_hi_d2494 = fdiv double %mul_hi_abcd_max2466, %zk_hi
  %force_round_fr_mem2495 = alloca double, align 8
  %629 = bitcast double* %force_round_fr_mem2495 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %629)
  store volatile double %div_hi_d2494, double* %force_round_fr_mem2495, align 8
  %force_roundfr_rounded2496 = load volatile double* %force_round_fr_mem2495, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %629)
  %div_hi_ab_test2497 = fcmp olt double %force_roundfr_rounded2487, %force_roundfr_rounded2490
  %div_hi_cd_test2498 = fcmp olt double %force_roundfr_rounded2493, %force_roundfr_rounded2496
  %div_hi_ab_max2499 = select i1 %div_hi_ab_test2497, double %force_roundfr_rounded2490, double %force_roundfr_rounded2487
  %div_hi_cd_max2500 = select i1 %div_hi_cd_test2498, double %force_roundfr_rounded2496, double %force_roundfr_rounded2493
  %div_hi_abcd_test2501 = fcmp olt double %div_hi_ab_max2499, %div_hi_cd_max2500
  %div_hi_abcd_max2502 = select i1 %div_hi_abcd_test2501, double %div_hi_cd_max2500, double %div_hi_ab_max2499
  call void @set_rounding_mode_down()
  %force_round_fr_mem2504 = alloca double, align 8
  %630 = bitcast double* %force_round_fr_mem2504 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %630)
  store volatile double %div_hi_a2485, double* %force_round_fr_mem2504, align 8
  %force_roundfr_rounded2505 = load volatile double* %force_round_fr_mem2504, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %630)
  %force_round_fr_mem2507 = alloca double, align 8
  %631 = bitcast double* %force_round_fr_mem2507 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %631)
  store volatile double %div_hi_b2488, double* %force_round_fr_mem2507, align 8
  %force_roundfr_rounded2508 = load volatile double* %force_round_fr_mem2507, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %631)
  %force_round_fr_mem2510 = alloca double, align 8
  %632 = bitcast double* %force_round_fr_mem2510 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %632)
  store volatile double %div_hi_c2491, double* %force_round_fr_mem2510, align 8
  %force_roundfr_rounded2511 = load volatile double* %force_round_fr_mem2510, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %632)
  %force_round_fr_mem2513 = alloca double, align 8
  %633 = bitcast double* %force_round_fr_mem2513 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %633)
  store volatile double %div_hi_d2494, double* %force_round_fr_mem2513, align 8
  %force_roundfr_rounded2514 = load volatile double* %force_round_fr_mem2513, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %633)
  %div_lo_ab_test2515 = fcmp olt double %force_roundfr_rounded2508, %force_roundfr_rounded2505
  %div_lo_cd_test2516 = fcmp olt double %force_roundfr_rounded2514, %force_roundfr_rounded2511
  %div_lo_cd_min2517 = select i1 %div_lo_cd_test2516, double %force_roundfr_rounded2514, double %force_roundfr_rounded2511
  %div_lo_ab_min2518 = select i1 %div_lo_ab_test2515, double %force_roundfr_rounded2508, double %force_roundfr_rounded2505
  %div_lo_abcd_test2519 = fcmp olt double %div_lo_cd_min2517, %div_lo_ab_min2518
  %div_lo_abcd_min2520 = select i1 %div_lo_abcd_test2519, double %div_lo_cd_min2517, double %div_lo_ab_min2518
  %add_lo2521 = fadd double %div_lo_abcd_min2520, %zj_lo
  %force_round_fr_mem2522 = alloca double, align 8
  %634 = bitcast double* %force_round_fr_mem2522 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %634)
  store volatile double %add_lo2521, double* %force_round_fr_mem2522, align 8
  %force_roundfr_rounded2523 = load volatile double* %force_round_fr_mem2522, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %634)
  call void @set_rounding_mode_up()
  %add_hi2524 = fadd double %div_hi_abcd_max2502, %zj_hi
  %force_round_fr_mem2525 = alloca double, align 8
  %635 = bitcast double* %force_round_fr_mem2525 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %635)
  store volatile double %add_hi2524, double* %force_round_fr_mem2525, align 8
  %force_roundfr_rounded2526 = load volatile double* %force_round_fr_mem2525, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %635)
  call void @set_rounding_mode_down()
  %add_lo2527 = fadd double %force_roundfr_rounded2523, %zi_lo
  %force_round_fr_mem2528 = alloca double, align 8
  %636 = bitcast double* %force_round_fr_mem2528 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %636)
  store volatile double %add_lo2527, double* %force_round_fr_mem2528, align 8
  %force_roundfr_rounded2529 = load volatile double* %force_round_fr_mem2528, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %636)
  call void @set_rounding_mode_up()
  %add_hi2530 = fadd double %force_roundfr_rounded2526, %zi_hi
  %force_round_fr_mem2531 = alloca double, align 8
  %637 = bitcast double* %force_round_fr_mem2531 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %637)
  store volatile double %add_hi2530, double* %force_round_fr_mem2531, align 8
  %force_roundfr_rounded2532 = load volatile double* %force_round_fr_mem2531, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %637)
  %mul_hi_a2533 = fmul double %force_roundfr_rounded2529, %zh_lo
  %force_round_fr_mem2534 = alloca double, align 8
  %638 = bitcast double* %force_round_fr_mem2534 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %638)
  store volatile double %mul_hi_a2533, double* %force_round_fr_mem2534, align 8
  %force_roundfr_rounded2535 = load volatile double* %force_round_fr_mem2534, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %638)
  %mul_hi_b2536 = fmul double %force_roundfr_rounded2529, %zh_hi
  %force_round_fr_mem2537 = alloca double, align 8
  %639 = bitcast double* %force_round_fr_mem2537 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %639)
  store volatile double %mul_hi_b2536, double* %force_round_fr_mem2537, align 8
  %force_roundfr_rounded2538 = load volatile double* %force_round_fr_mem2537, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %639)
  %mul_hi_c2539 = fmul double %force_roundfr_rounded2532, %zh_lo
  %force_round_fr_mem2540 = alloca double, align 8
  %640 = bitcast double* %force_round_fr_mem2540 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %640)
  store volatile double %mul_hi_c2539, double* %force_round_fr_mem2540, align 8
  %force_roundfr_rounded2541 = load volatile double* %force_round_fr_mem2540, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %640)
  %mul_hi_d2542 = fmul double %force_roundfr_rounded2532, %zh_hi
  %force_round_fr_mem2543 = alloca double, align 8
  %641 = bitcast double* %force_round_fr_mem2543 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %641)
  store volatile double %mul_hi_d2542, double* %force_round_fr_mem2543, align 8
  %force_roundfr_rounded2544 = load volatile double* %force_round_fr_mem2543, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %641)
  %mul_hi_ab_test2545 = fcmp olt double %force_roundfr_rounded2535, %force_roundfr_rounded2538
  %mul_hi_cd_test2546 = fcmp olt double %force_roundfr_rounded2541, %force_roundfr_rounded2544
  %mul_hi_ab_max2547 = select i1 %mul_hi_ab_test2545, double %force_roundfr_rounded2538, double %force_roundfr_rounded2535
  %mul_hi_cd_max2548 = select i1 %mul_hi_cd_test2546, double %force_roundfr_rounded2544, double %force_roundfr_rounded2541
  %mul_hi_abcd_test2549 = fcmp olt double %mul_hi_ab_max2547, %mul_hi_cd_max2548
  %mul_hi_abcd_max2550 = select i1 %mul_hi_abcd_test2549, double %mul_hi_cd_max2548, double %mul_hi_ab_max2547
  call void @set_rounding_mode_down()
  %force_round_fr_mem2552 = alloca double, align 8
  %642 = bitcast double* %force_round_fr_mem2552 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %642)
  store volatile double %mul_hi_a2533, double* %force_round_fr_mem2552, align 8
  %force_roundfr_rounded2553 = load volatile double* %force_round_fr_mem2552, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %642)
  %force_round_fr_mem2555 = alloca double, align 8
  %643 = bitcast double* %force_round_fr_mem2555 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %643)
  store volatile double %mul_hi_b2536, double* %force_round_fr_mem2555, align 8
  %force_roundfr_rounded2556 = load volatile double* %force_round_fr_mem2555, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %643)
  %force_round_fr_mem2558 = alloca double, align 8
  %644 = bitcast double* %force_round_fr_mem2558 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %644)
  store volatile double %mul_hi_c2539, double* %force_round_fr_mem2558, align 8
  %force_roundfr_rounded2559 = load volatile double* %force_round_fr_mem2558, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %644)
  %force_round_fr_mem2561 = alloca double, align 8
  %645 = bitcast double* %force_round_fr_mem2561 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %645)
  store volatile double %mul_hi_d2542, double* %force_round_fr_mem2561, align 8
  %force_roundfr_rounded2562 = load volatile double* %force_round_fr_mem2561, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %645)
  %mul_lo_ab_test2563 = fcmp olt double %force_roundfr_rounded2556, %force_roundfr_rounded2553
  %mul_lo_cd_test2564 = fcmp olt double %force_roundfr_rounded2562, %force_roundfr_rounded2559
  %mul_lo_cd_min2565 = select i1 %mul_lo_cd_test2564, double %force_roundfr_rounded2562, double %force_roundfr_rounded2559
  %mul_lo_ab_min2566 = select i1 %mul_lo_ab_test2563, double %force_roundfr_rounded2556, double %force_roundfr_rounded2553
  %mul_lo_abcd_test2567 = fcmp olt double %mul_lo_cd_min2565, %mul_lo_ab_min2566
  %mul_lo_abcd_min2568 = select i1 %mul_lo_abcd_test2567, double %mul_lo_cd_min2565, double %mul_lo_ab_min2566
  call void @set_rounding_mode_up()
  %div_hi_a2569 = fdiv double %mul_lo_abcd_min2568, %zg_lo
  %force_round_fr_mem2570 = alloca double, align 8
  %646 = bitcast double* %force_round_fr_mem2570 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %646)
  store volatile double %div_hi_a2569, double* %force_round_fr_mem2570, align 8
  %force_roundfr_rounded2571 = load volatile double* %force_round_fr_mem2570, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %646)
  %div_hi_b2572 = fdiv double %mul_lo_abcd_min2568, %zg_hi
  %force_round_fr_mem2573 = alloca double, align 8
  %647 = bitcast double* %force_round_fr_mem2573 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %647)
  store volatile double %div_hi_b2572, double* %force_round_fr_mem2573, align 8
  %force_roundfr_rounded2574 = load volatile double* %force_round_fr_mem2573, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %647)
  %div_hi_c2575 = fdiv double %mul_hi_abcd_max2550, %zg_lo
  %force_round_fr_mem2576 = alloca double, align 8
  %648 = bitcast double* %force_round_fr_mem2576 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %648)
  store volatile double %div_hi_c2575, double* %force_round_fr_mem2576, align 8
  %force_roundfr_rounded2577 = load volatile double* %force_round_fr_mem2576, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %648)
  %div_hi_d2578 = fdiv double %mul_hi_abcd_max2550, %zg_hi
  %force_round_fr_mem2579 = alloca double, align 8
  %649 = bitcast double* %force_round_fr_mem2579 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %649)
  store volatile double %div_hi_d2578, double* %force_round_fr_mem2579, align 8
  %force_roundfr_rounded2580 = load volatile double* %force_round_fr_mem2579, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %649)
  %div_hi_ab_test2581 = fcmp olt double %force_roundfr_rounded2571, %force_roundfr_rounded2574
  %div_hi_cd_test2582 = fcmp olt double %force_roundfr_rounded2577, %force_roundfr_rounded2580
  %div_hi_ab_max2583 = select i1 %div_hi_ab_test2581, double %force_roundfr_rounded2574, double %force_roundfr_rounded2571
  %div_hi_cd_max2584 = select i1 %div_hi_cd_test2582, double %force_roundfr_rounded2580, double %force_roundfr_rounded2577
  %div_hi_abcd_test2585 = fcmp olt double %div_hi_ab_max2583, %div_hi_cd_max2584
  %div_hi_abcd_max2586 = select i1 %div_hi_abcd_test2585, double %div_hi_cd_max2584, double %div_hi_ab_max2583
  call void @set_rounding_mode_down()
  %force_round_fr_mem2588 = alloca double, align 8
  %650 = bitcast double* %force_round_fr_mem2588 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %650)
  store volatile double %div_hi_a2569, double* %force_round_fr_mem2588, align 8
  %force_roundfr_rounded2589 = load volatile double* %force_round_fr_mem2588, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %650)
  %force_round_fr_mem2591 = alloca double, align 8
  %651 = bitcast double* %force_round_fr_mem2591 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %651)
  store volatile double %div_hi_b2572, double* %force_round_fr_mem2591, align 8
  %force_roundfr_rounded2592 = load volatile double* %force_round_fr_mem2591, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %651)
  %force_round_fr_mem2594 = alloca double, align 8
  %652 = bitcast double* %force_round_fr_mem2594 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %652)
  store volatile double %div_hi_c2575, double* %force_round_fr_mem2594, align 8
  %force_roundfr_rounded2595 = load volatile double* %force_round_fr_mem2594, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %652)
  %force_round_fr_mem2597 = alloca double, align 8
  %653 = bitcast double* %force_round_fr_mem2597 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %653)
  store volatile double %div_hi_d2578, double* %force_round_fr_mem2597, align 8
  %force_roundfr_rounded2598 = load volatile double* %force_round_fr_mem2597, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %653)
  %div_lo_ab_test2599 = fcmp olt double %force_roundfr_rounded2592, %force_roundfr_rounded2589
  %div_lo_cd_test2600 = fcmp olt double %force_roundfr_rounded2598, %force_roundfr_rounded2595
  %div_lo_cd_min2601 = select i1 %div_lo_cd_test2600, double %force_roundfr_rounded2598, double %force_roundfr_rounded2595
  %div_lo_ab_min2602 = select i1 %div_lo_ab_test2599, double %force_roundfr_rounded2592, double %force_roundfr_rounded2589
  %div_lo_abcd_test2603 = fcmp olt double %div_lo_cd_min2601, %div_lo_ab_min2602
  %div_lo_abcd_min2604 = select i1 %div_lo_abcd_test2603, double %div_lo_cd_min2601, double %div_lo_ab_min2602
  %sub_lo2605 = fsub double %zf_lo, %div_hi_abcd_max2586
  %force_round_fr_mem2606 = alloca double, align 8
  %654 = bitcast double* %force_round_fr_mem2606 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %654)
  store volatile double %sub_lo2605, double* %force_round_fr_mem2606, align 8
  %force_roundfr_rounded2607 = load volatile double* %force_round_fr_mem2606, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %654)
  call void @set_rounding_mode_up()
  %sub_hi2608 = fsub double %zf_hi, %div_lo_abcd_min2604
  %force_round_fr_mem2609 = alloca double, align 8
  %655 = bitcast double* %force_round_fr_mem2609 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %655)
  store volatile double %sub_hi2608, double* %force_round_fr_mem2609, align 8
  %force_roundfr_rounded2610 = load volatile double* %force_round_fr_mem2609, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %655)
  %div_hi_a2611 = fdiv double %ze_lo, %force_roundfr_rounded2607
  %force_round_fr_mem2612 = alloca double, align 8
  %656 = bitcast double* %force_round_fr_mem2612 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %656)
  store volatile double %div_hi_a2611, double* %force_round_fr_mem2612, align 8
  %force_roundfr_rounded2613 = load volatile double* %force_round_fr_mem2612, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %656)
  %div_hi_b2614 = fdiv double %ze_lo, %force_roundfr_rounded2610
  %force_round_fr_mem2615 = alloca double, align 8
  %657 = bitcast double* %force_round_fr_mem2615 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %657)
  store volatile double %div_hi_b2614, double* %force_round_fr_mem2615, align 8
  %force_roundfr_rounded2616 = load volatile double* %force_round_fr_mem2615, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %657)
  %div_hi_c2617 = fdiv double %ze_hi, %force_roundfr_rounded2607
  %force_round_fr_mem2618 = alloca double, align 8
  %658 = bitcast double* %force_round_fr_mem2618 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %658)
  store volatile double %div_hi_c2617, double* %force_round_fr_mem2618, align 8
  %force_roundfr_rounded2619 = load volatile double* %force_round_fr_mem2618, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %658)
  %div_hi_d2620 = fdiv double %ze_hi, %force_roundfr_rounded2610
  %force_round_fr_mem2621 = alloca double, align 8
  %659 = bitcast double* %force_round_fr_mem2621 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %659)
  store volatile double %div_hi_d2620, double* %force_round_fr_mem2621, align 8
  %force_roundfr_rounded2622 = load volatile double* %force_round_fr_mem2621, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %659)
  %div_hi_ab_test2623 = fcmp olt double %force_roundfr_rounded2613, %force_roundfr_rounded2616
  %div_hi_cd_test2624 = fcmp olt double %force_roundfr_rounded2619, %force_roundfr_rounded2622
  %div_hi_ab_max2625 = select i1 %div_hi_ab_test2623, double %force_roundfr_rounded2616, double %force_roundfr_rounded2613
  %div_hi_cd_max2626 = select i1 %div_hi_cd_test2624, double %force_roundfr_rounded2622, double %force_roundfr_rounded2619
  %div_hi_abcd_test2627 = fcmp olt double %div_hi_ab_max2625, %div_hi_cd_max2626
  %div_hi_abcd_max2628 = select i1 %div_hi_abcd_test2627, double %div_hi_cd_max2626, double %div_hi_ab_max2625
  call void @set_rounding_mode_down()
  %force_round_fr_mem2630 = alloca double, align 8
  %660 = bitcast double* %force_round_fr_mem2630 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %660)
  store volatile double %div_hi_a2611, double* %force_round_fr_mem2630, align 8
  %force_roundfr_rounded2631 = load volatile double* %force_round_fr_mem2630, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %660)
  %force_round_fr_mem2633 = alloca double, align 8
  %661 = bitcast double* %force_round_fr_mem2633 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %661)
  store volatile double %div_hi_b2614, double* %force_round_fr_mem2633, align 8
  %force_roundfr_rounded2634 = load volatile double* %force_round_fr_mem2633, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %661)
  %force_round_fr_mem2636 = alloca double, align 8
  %662 = bitcast double* %force_round_fr_mem2636 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %662)
  store volatile double %div_hi_c2617, double* %force_round_fr_mem2636, align 8
  %force_roundfr_rounded2637 = load volatile double* %force_round_fr_mem2636, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %662)
  %force_round_fr_mem2639 = alloca double, align 8
  %663 = bitcast double* %force_round_fr_mem2639 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %663)
  store volatile double %div_hi_d2620, double* %force_round_fr_mem2639, align 8
  %force_roundfr_rounded2640 = load volatile double* %force_round_fr_mem2639, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %663)
  %div_lo_ab_test2641 = fcmp olt double %force_roundfr_rounded2634, %force_roundfr_rounded2631
  %div_lo_cd_test2642 = fcmp olt double %force_roundfr_rounded2640, %force_roundfr_rounded2637
  %div_lo_cd_min2643 = select i1 %div_lo_cd_test2642, double %force_roundfr_rounded2640, double %force_roundfr_rounded2637
  %div_lo_ab_min2644 = select i1 %div_lo_ab_test2641, double %force_roundfr_rounded2634, double %force_roundfr_rounded2631
  %div_lo_abcd_test2645 = fcmp olt double %div_lo_cd_min2643, %div_lo_ab_min2644
  %div_lo_abcd_min2646 = select i1 %div_lo_abcd_test2645, double %div_lo_cd_min2643, double %div_lo_ab_min2644
  call void @set_rounding_mode_up()
  %mul_hi_a2647 = fmul double %div_lo_abcd_min2646, %zd_lo
  %force_round_fr_mem2648 = alloca double, align 8
  %664 = bitcast double* %force_round_fr_mem2648 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %664)
  store volatile double %mul_hi_a2647, double* %force_round_fr_mem2648, align 8
  %force_roundfr_rounded2649 = load volatile double* %force_round_fr_mem2648, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %664)
  %mul_hi_b2650 = fmul double %div_lo_abcd_min2646, %zd_hi
  %force_round_fr_mem2651 = alloca double, align 8
  %665 = bitcast double* %force_round_fr_mem2651 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %665)
  store volatile double %mul_hi_b2650, double* %force_round_fr_mem2651, align 8
  %force_roundfr_rounded2652 = load volatile double* %force_round_fr_mem2651, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %665)
  %mul_hi_c2653 = fmul double %div_hi_abcd_max2628, %zd_lo
  %force_round_fr_mem2654 = alloca double, align 8
  %666 = bitcast double* %force_round_fr_mem2654 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %666)
  store volatile double %mul_hi_c2653, double* %force_round_fr_mem2654, align 8
  %force_roundfr_rounded2655 = load volatile double* %force_round_fr_mem2654, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %666)
  %mul_hi_d2656 = fmul double %div_hi_abcd_max2628, %zd_hi
  %force_round_fr_mem2657 = alloca double, align 8
  %667 = bitcast double* %force_round_fr_mem2657 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %667)
  store volatile double %mul_hi_d2656, double* %force_round_fr_mem2657, align 8
  %force_roundfr_rounded2658 = load volatile double* %force_round_fr_mem2657, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %667)
  %mul_hi_ab_test2659 = fcmp olt double %force_roundfr_rounded2649, %force_roundfr_rounded2652
  %mul_hi_cd_test2660 = fcmp olt double %force_roundfr_rounded2655, %force_roundfr_rounded2658
  %mul_hi_ab_max2661 = select i1 %mul_hi_ab_test2659, double %force_roundfr_rounded2652, double %force_roundfr_rounded2649
  %mul_hi_cd_max2662 = select i1 %mul_hi_cd_test2660, double %force_roundfr_rounded2658, double %force_roundfr_rounded2655
  %mul_hi_abcd_test2663 = fcmp olt double %mul_hi_ab_max2661, %mul_hi_cd_max2662
  %mul_hi_abcd_max2664 = select i1 %mul_hi_abcd_test2663, double %mul_hi_cd_max2662, double %mul_hi_ab_max2661
  call void @set_rounding_mode_down()
  %force_round_fr_mem2666 = alloca double, align 8
  %668 = bitcast double* %force_round_fr_mem2666 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %668)
  store volatile double %mul_hi_a2647, double* %force_round_fr_mem2666, align 8
  %force_roundfr_rounded2667 = load volatile double* %force_round_fr_mem2666, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %668)
  %force_round_fr_mem2669 = alloca double, align 8
  %669 = bitcast double* %force_round_fr_mem2669 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %669)
  store volatile double %mul_hi_b2650, double* %force_round_fr_mem2669, align 8
  %force_roundfr_rounded2670 = load volatile double* %force_round_fr_mem2669, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %669)
  %force_round_fr_mem2672 = alloca double, align 8
  %670 = bitcast double* %force_round_fr_mem2672 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %670)
  store volatile double %mul_hi_c2653, double* %force_round_fr_mem2672, align 8
  %force_roundfr_rounded2673 = load volatile double* %force_round_fr_mem2672, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %670)
  %force_round_fr_mem2675 = alloca double, align 8
  %671 = bitcast double* %force_round_fr_mem2675 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %671)
  store volatile double %mul_hi_d2656, double* %force_round_fr_mem2675, align 8
  %force_roundfr_rounded2676 = load volatile double* %force_round_fr_mem2675, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %671)
  %mul_lo_ab_test2677 = fcmp olt double %force_roundfr_rounded2670, %force_roundfr_rounded2667
  %mul_lo_cd_test2678 = fcmp olt double %force_roundfr_rounded2676, %force_roundfr_rounded2673
  %mul_lo_cd_min2679 = select i1 %mul_lo_cd_test2678, double %force_roundfr_rounded2676, double %force_roundfr_rounded2673
  %mul_lo_ab_min2680 = select i1 %mul_lo_ab_test2677, double %force_roundfr_rounded2670, double %force_roundfr_rounded2667
  %mul_lo_abcd_test2681 = fcmp olt double %mul_lo_cd_min2679, %mul_lo_ab_min2680
  %mul_lo_abcd_min2682 = select i1 %mul_lo_abcd_test2681, double %mul_lo_cd_min2679, double %mul_lo_ab_min2680
  call void @set_rounding_mode_up()
  %div_hi_a2683 = fdiv double %zc_lo, %mul_lo_abcd_min2682
  %force_round_fr_mem2684 = alloca double, align 8
  %672 = bitcast double* %force_round_fr_mem2684 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %672)
  store volatile double %div_hi_a2683, double* %force_round_fr_mem2684, align 8
  %force_roundfr_rounded2685 = load volatile double* %force_round_fr_mem2684, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %672)
  %div_hi_b2686 = fdiv double %zc_lo, %mul_hi_abcd_max2664
  %force_round_fr_mem2687 = alloca double, align 8
  %673 = bitcast double* %force_round_fr_mem2687 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %673)
  store volatile double %div_hi_b2686, double* %force_round_fr_mem2687, align 8
  %force_roundfr_rounded2688 = load volatile double* %force_round_fr_mem2687, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %673)
  %div_hi_c2689 = fdiv double %zc_hi, %mul_lo_abcd_min2682
  %force_round_fr_mem2690 = alloca double, align 8
  %674 = bitcast double* %force_round_fr_mem2690 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %674)
  store volatile double %div_hi_c2689, double* %force_round_fr_mem2690, align 8
  %force_roundfr_rounded2691 = load volatile double* %force_round_fr_mem2690, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %674)
  %div_hi_d2692 = fdiv double %zc_hi, %mul_hi_abcd_max2664
  %force_round_fr_mem2693 = alloca double, align 8
  %675 = bitcast double* %force_round_fr_mem2693 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %675)
  store volatile double %div_hi_d2692, double* %force_round_fr_mem2693, align 8
  %force_roundfr_rounded2694 = load volatile double* %force_round_fr_mem2693, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %675)
  %div_hi_ab_test2695 = fcmp olt double %force_roundfr_rounded2685, %force_roundfr_rounded2688
  %div_hi_cd_test2696 = fcmp olt double %force_roundfr_rounded2691, %force_roundfr_rounded2694
  %div_hi_ab_max2697 = select i1 %div_hi_ab_test2695, double %force_roundfr_rounded2688, double %force_roundfr_rounded2685
  %div_hi_cd_max2698 = select i1 %div_hi_cd_test2696, double %force_roundfr_rounded2694, double %force_roundfr_rounded2691
  %div_hi_abcd_test2699 = fcmp olt double %div_hi_ab_max2697, %div_hi_cd_max2698
  %div_hi_abcd_max2700 = select i1 %div_hi_abcd_test2699, double %div_hi_cd_max2698, double %div_hi_ab_max2697
  call void @set_rounding_mode_down()
  %force_round_fr_mem2702 = alloca double, align 8
  %676 = bitcast double* %force_round_fr_mem2702 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %676)
  store volatile double %div_hi_a2683, double* %force_round_fr_mem2702, align 8
  %force_roundfr_rounded2703 = load volatile double* %force_round_fr_mem2702, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %676)
  %force_round_fr_mem2705 = alloca double, align 8
  %677 = bitcast double* %force_round_fr_mem2705 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %677)
  store volatile double %div_hi_b2686, double* %force_round_fr_mem2705, align 8
  %force_roundfr_rounded2706 = load volatile double* %force_round_fr_mem2705, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %677)
  %force_round_fr_mem2708 = alloca double, align 8
  %678 = bitcast double* %force_round_fr_mem2708 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %678)
  store volatile double %div_hi_c2689, double* %force_round_fr_mem2708, align 8
  %force_roundfr_rounded2709 = load volatile double* %force_round_fr_mem2708, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %678)
  %force_round_fr_mem2711 = alloca double, align 8
  %679 = bitcast double* %force_round_fr_mem2711 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %679)
  store volatile double %div_hi_d2692, double* %force_round_fr_mem2711, align 8
  %force_roundfr_rounded2712 = load volatile double* %force_round_fr_mem2711, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %679)
  %div_lo_ab_test2713 = fcmp olt double %force_roundfr_rounded2706, %force_roundfr_rounded2703
  %div_lo_cd_test2714 = fcmp olt double %force_roundfr_rounded2712, %force_roundfr_rounded2709
  %div_lo_cd_min2715 = select i1 %div_lo_cd_test2714, double %force_roundfr_rounded2712, double %force_roundfr_rounded2709
  %div_lo_ab_min2716 = select i1 %div_lo_ab_test2713, double %force_roundfr_rounded2706, double %force_roundfr_rounded2703
  %div_lo_abcd_test2717 = fcmp olt double %div_lo_cd_min2715, %div_lo_ab_min2716
  %div_lo_abcd_min2718 = select i1 %div_lo_abcd_test2717, double %div_lo_cd_min2715, double %div_lo_ab_min2716
  call void @set_rounding_mode_up()
  %mul_hi_a2719 = fmul double %div_lo_abcd_min2718, %zb_lo
  %force_round_fr_mem2720 = alloca double, align 8
  %680 = bitcast double* %force_round_fr_mem2720 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %680)
  store volatile double %mul_hi_a2719, double* %force_round_fr_mem2720, align 8
  %force_roundfr_rounded2721 = load volatile double* %force_round_fr_mem2720, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %680)
  %mul_hi_b2722 = fmul double %div_hi_abcd_max2700, %zb_lo
  %force_round_fr_mem2723 = alloca double, align 8
  %681 = bitcast double* %force_round_fr_mem2723 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %681)
  store volatile double %mul_hi_b2722, double* %force_round_fr_mem2723, align 8
  %force_roundfr_rounded2724 = load volatile double* %force_round_fr_mem2723, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %681)
  %mul_hi_c2725 = fmul double %div_lo_abcd_min2718, %zb_hi
  %force_round_fr_mem2726 = alloca double, align 8
  %682 = bitcast double* %force_round_fr_mem2726 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %682)
  store volatile double %mul_hi_c2725, double* %force_round_fr_mem2726, align 8
  %force_roundfr_rounded2727 = load volatile double* %force_round_fr_mem2726, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %682)
  %mul_hi_d2728 = fmul double %div_hi_abcd_max2700, %zb_hi
  %force_round_fr_mem2729 = alloca double, align 8
  %683 = bitcast double* %force_round_fr_mem2729 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %683)
  store volatile double %mul_hi_d2728, double* %force_round_fr_mem2729, align 8
  %force_roundfr_rounded2730 = load volatile double* %force_round_fr_mem2729, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %683)
  %mul_hi_ab_test2731 = fcmp olt double %force_roundfr_rounded2721, %force_roundfr_rounded2724
  %mul_hi_cd_test2732 = fcmp olt double %force_roundfr_rounded2727, %force_roundfr_rounded2730
  %mul_hi_ab_max2733 = select i1 %mul_hi_ab_test2731, double %force_roundfr_rounded2724, double %force_roundfr_rounded2721
  %mul_hi_cd_max2734 = select i1 %mul_hi_cd_test2732, double %force_roundfr_rounded2730, double %force_roundfr_rounded2727
  %mul_hi_abcd_test2735 = fcmp olt double %mul_hi_ab_max2733, %mul_hi_cd_max2734
  %mul_hi_abcd_max2736 = select i1 %mul_hi_abcd_test2735, double %mul_hi_cd_max2734, double %mul_hi_ab_max2733
  call void @set_rounding_mode_down()
  %force_round_fr_mem2738 = alloca double, align 8
  %684 = bitcast double* %force_round_fr_mem2738 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %684)
  store volatile double %mul_hi_a2719, double* %force_round_fr_mem2738, align 8
  %force_roundfr_rounded2739 = load volatile double* %force_round_fr_mem2738, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %684)
  %force_round_fr_mem2741 = alloca double, align 8
  %685 = bitcast double* %force_round_fr_mem2741 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %685)
  store volatile double %mul_hi_b2722, double* %force_round_fr_mem2741, align 8
  %force_roundfr_rounded2742 = load volatile double* %force_round_fr_mem2741, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %685)
  %force_round_fr_mem2744 = alloca double, align 8
  %686 = bitcast double* %force_round_fr_mem2744 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %686)
  store volatile double %mul_hi_c2725, double* %force_round_fr_mem2744, align 8
  %force_roundfr_rounded2745 = load volatile double* %force_round_fr_mem2744, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %686)
  %force_round_fr_mem2747 = alloca double, align 8
  %687 = bitcast double* %force_round_fr_mem2747 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %687)
  store volatile double %mul_hi_d2728, double* %force_round_fr_mem2747, align 8
  %force_roundfr_rounded2748 = load volatile double* %force_round_fr_mem2747, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %687)
  %mul_lo_ab_test2749 = fcmp olt double %force_roundfr_rounded2742, %force_roundfr_rounded2739
  %mul_lo_cd_test2750 = fcmp olt double %force_roundfr_rounded2748, %force_roundfr_rounded2745
  %mul_lo_cd_min2751 = select i1 %mul_lo_cd_test2750, double %force_roundfr_rounded2748, double %force_roundfr_rounded2745
  %mul_lo_ab_min2752 = select i1 %mul_lo_ab_test2749, double %force_roundfr_rounded2742, double %force_roundfr_rounded2739
  %mul_lo_abcd_test2753 = fcmp olt double %mul_lo_cd_min2751, %mul_lo_ab_min2752
  %mul_lo_abcd_min2754 = select i1 %mul_lo_abcd_test2753, double %mul_lo_cd_min2751, double %mul_lo_ab_min2752
  call void @set_rounding_mode_up()
  %mul_hi_a2755 = fmul double %mul_lo_abcd_min2754, %za_lo
  %force_round_fr_mem2756 = alloca double, align 8
  %688 = bitcast double* %force_round_fr_mem2756 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %688)
  store volatile double %mul_hi_a2755, double* %force_round_fr_mem2756, align 8
  %force_roundfr_rounded2757 = load volatile double* %force_round_fr_mem2756, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %688)
  %mul_hi_b2758 = fmul double %mul_lo_abcd_min2754, %za_hi
  %force_round_fr_mem2759 = alloca double, align 8
  %689 = bitcast double* %force_round_fr_mem2759 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %689)
  store volatile double %mul_hi_b2758, double* %force_round_fr_mem2759, align 8
  %force_roundfr_rounded2760 = load volatile double* %force_round_fr_mem2759, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %689)
  %mul_hi_c2761 = fmul double %mul_hi_abcd_max2736, %za_lo
  %force_round_fr_mem2762 = alloca double, align 8
  %690 = bitcast double* %force_round_fr_mem2762 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %690)
  store volatile double %mul_hi_c2761, double* %force_round_fr_mem2762, align 8
  %force_roundfr_rounded2763 = load volatile double* %force_round_fr_mem2762, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %690)
  %mul_hi_d2764 = fmul double %mul_hi_abcd_max2736, %za_hi
  %force_round_fr_mem2765 = alloca double, align 8
  %691 = bitcast double* %force_round_fr_mem2765 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %691)
  store volatile double %mul_hi_d2764, double* %force_round_fr_mem2765, align 8
  %force_roundfr_rounded2766 = load volatile double* %force_round_fr_mem2765, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %691)
  %mul_hi_ab_test2767 = fcmp olt double %force_roundfr_rounded2757, %force_roundfr_rounded2760
  %mul_hi_cd_test2768 = fcmp olt double %force_roundfr_rounded2763, %force_roundfr_rounded2766
  %mul_hi_ab_max2769 = select i1 %mul_hi_ab_test2767, double %force_roundfr_rounded2760, double %force_roundfr_rounded2757
  %mul_hi_cd_max2770 = select i1 %mul_hi_cd_test2768, double %force_roundfr_rounded2766, double %force_roundfr_rounded2763
  %mul_hi_abcd_test2771 = fcmp olt double %mul_hi_ab_max2769, %mul_hi_cd_max2770
  %mul_hi_abcd_max2772 = select i1 %mul_hi_abcd_test2771, double %mul_hi_cd_max2770, double %mul_hi_ab_max2769
  call void @set_rounding_mode_down()
  %force_round_fr_mem2774 = alloca double, align 8
  %692 = bitcast double* %force_round_fr_mem2774 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %692)
  store volatile double %mul_hi_a2755, double* %force_round_fr_mem2774, align 8
  %force_roundfr_rounded2775 = load volatile double* %force_round_fr_mem2774, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %692)
  %force_round_fr_mem2777 = alloca double, align 8
  %693 = bitcast double* %force_round_fr_mem2777 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %693)
  store volatile double %mul_hi_b2758, double* %force_round_fr_mem2777, align 8
  %force_roundfr_rounded2778 = load volatile double* %force_round_fr_mem2777, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %693)
  %force_round_fr_mem2780 = alloca double, align 8
  %694 = bitcast double* %force_round_fr_mem2780 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %694)
  store volatile double %mul_hi_c2761, double* %force_round_fr_mem2780, align 8
  %force_roundfr_rounded2781 = load volatile double* %force_round_fr_mem2780, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %694)
  %force_round_fr_mem2783 = alloca double, align 8
  %695 = bitcast double* %force_round_fr_mem2783 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %695)
  store volatile double %mul_hi_d2764, double* %force_round_fr_mem2783, align 8
  %force_roundfr_rounded2784 = load volatile double* %force_round_fr_mem2783, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %695)
  %mul_lo_ab_test2785 = fcmp olt double %force_roundfr_rounded2778, %force_roundfr_rounded2775
  %mul_lo_cd_test2786 = fcmp olt double %force_roundfr_rounded2784, %force_roundfr_rounded2781
  %mul_lo_cd_min2787 = select i1 %mul_lo_cd_test2786, double %force_roundfr_rounded2784, double %force_roundfr_rounded2781
  %mul_lo_ab_min2788 = select i1 %mul_lo_ab_test2785, double %force_roundfr_rounded2778, double %force_roundfr_rounded2775
  %mul_lo_abcd_test2789 = fcmp olt double %mul_lo_cd_min2787, %mul_lo_ab_min2788
  %mul_lo_abcd_min2790 = select i1 %mul_lo_abcd_test2789, double %mul_lo_cd_min2787, double %mul_lo_ab_min2788
  %func_res_interval_init = insertvalue { double, double } undef, double %mul_lo_abcd_min2790, 0
  %func_res_interval = insertvalue { double, double } %func_res_interval_init, double %mul_hi_abcd_max2772, 1
  ret { double, double } %func_res_interval
}