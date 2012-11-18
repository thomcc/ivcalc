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

define { double, double } @func2(double %a_lo, double %a_hi, double %b_lo, double %b_hi, double %c_lo, double %c_hi, double %d_lo, double %d_hi, double %e_lo, double %e_hi, double %f_lo, double %f_hi, double %g_lo, double %g_hi, double %h_lo, double %h_hi, double %i_lo, double %i_hi) {
entry:
  call void @set_rounding_mode_down()
  %add_lo = fadd double %a_lo, %b_lo
  %force_round_fr_mem = alloca double, align 8
  %0 = bitcast double* %force_round_fr_mem to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %0)
  store volatile double %add_lo, double* %force_round_fr_mem, align 8
  %force_roundfr_rounded = load volatile double* %force_round_fr_mem, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %0)
  call void @set_rounding_mode_up()
  %add_hi = fadd double %a_hi, %b_hi
  %force_round_fr_mem3 = alloca double, align 8
  %1 = bitcast double* %force_round_fr_mem3 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %1)
  store volatile double %add_hi, double* %force_round_fr_mem3, align 8
  %force_roundfr_rounded4 = load volatile double* %force_round_fr_mem3, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %1)
  %div_hi_a = fdiv double 2.300000e+01, %force_roundfr_rounded
  %force_round_fr_mem5 = alloca double, align 8
  %2 = bitcast double* %force_round_fr_mem5 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %2)
  store volatile double %div_hi_a, double* %force_round_fr_mem5, align 8
  %force_roundfr_rounded6 = load volatile double* %force_round_fr_mem5, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %2)
  %div_hi_b = fdiv double 2.300000e+01, %force_roundfr_rounded4
  %force_round_fr_mem7 = alloca double, align 8
  %3 = bitcast double* %force_round_fr_mem7 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %3)
  store volatile double %div_hi_b, double* %force_round_fr_mem7, align 8
  %force_roundfr_rounded8 = load volatile double* %force_round_fr_mem7, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %3)
  %force_round_fr_mem9 = alloca double, align 8
  %4 = bitcast double* %force_round_fr_mem9 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %4)
  store volatile double %div_hi_a, double* %force_round_fr_mem9, align 8
  %force_roundfr_rounded10 = load volatile double* %force_round_fr_mem9, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %4)
  %force_round_fr_mem11 = alloca double, align 8
  %5 = bitcast double* %force_round_fr_mem11 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %5)
  store volatile double %div_hi_b, double* %force_round_fr_mem11, align 8
  %force_roundfr_rounded12 = load volatile double* %force_round_fr_mem11, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %5)
  %div_hi_ab_test = fcmp olt double %force_roundfr_rounded6, %force_roundfr_rounded8
  %div_hi_cd_test = fcmp olt double %force_roundfr_rounded10, %force_roundfr_rounded12
  %div_hi_ab_max = select i1 %div_hi_ab_test, double %force_roundfr_rounded8, double %force_roundfr_rounded6
  %div_hi_cd_max = select i1 %div_hi_cd_test, double %force_roundfr_rounded12, double %force_roundfr_rounded10
  %div_hi_abcd_test = fcmp olt double %div_hi_ab_max, %div_hi_cd_max
  %div_hi_abcd_max = select i1 %div_hi_abcd_test, double %div_hi_cd_max, double %div_hi_ab_max
  call void @set_rounding_mode_down()
  %force_round_fr_mem13 = alloca double, align 8
  %6 = bitcast double* %force_round_fr_mem13 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %6)
  store volatile double %div_hi_a, double* %force_round_fr_mem13, align 8
  %force_roundfr_rounded14 = load volatile double* %force_round_fr_mem13, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %6)
  %force_round_fr_mem15 = alloca double, align 8
  %7 = bitcast double* %force_round_fr_mem15 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %7)
  store volatile double %div_hi_b, double* %force_round_fr_mem15, align 8
  %force_roundfr_rounded16 = load volatile double* %force_round_fr_mem15, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %7)
  %force_round_fr_mem17 = alloca double, align 8
  %8 = bitcast double* %force_round_fr_mem17 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %8)
  store volatile double %div_hi_a, double* %force_round_fr_mem17, align 8
  %force_roundfr_rounded18 = load volatile double* %force_round_fr_mem17, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %8)
  %force_round_fr_mem19 = alloca double, align 8
  %9 = bitcast double* %force_round_fr_mem19 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %9)
  store volatile double %div_hi_b, double* %force_round_fr_mem19, align 8
  %force_roundfr_rounded20 = load volatile double* %force_round_fr_mem19, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %9)
  %div_lo_ab_test = fcmp olt double %force_roundfr_rounded16, %force_roundfr_rounded14
  %div_lo_cd_test = fcmp olt double %force_roundfr_rounded20, %force_roundfr_rounded18
  %div_lo_cd_min = select i1 %div_lo_cd_test, double %force_roundfr_rounded20, double %force_roundfr_rounded18
  %div_lo_ab_min = select i1 %div_lo_ab_test, double %force_roundfr_rounded16, double %force_roundfr_rounded14
  %div_lo_abcd_test = fcmp olt double %div_lo_cd_min, %div_lo_ab_min
  %div_lo_abcd_min = select i1 %div_lo_abcd_test, double %div_lo_cd_min, double %div_lo_ab_min
  call void @set_rounding_mode_up()
  %mul_hi_a = fmul double %div_lo_abcd_min, -6.000000e+00
  %force_round_fr_mem23 = alloca double, align 8
  %10 = bitcast double* %force_round_fr_mem23 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %10)
  store volatile double %mul_hi_a, double* %force_round_fr_mem23, align 8
  %force_roundfr_rounded24 = load volatile double* %force_round_fr_mem23, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %10)
  %force_round_fr_mem25 = alloca double, align 8
  %11 = bitcast double* %force_round_fr_mem25 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %11)
  store volatile double %mul_hi_a, double* %force_round_fr_mem25, align 8
  %force_roundfr_rounded26 = load volatile double* %force_round_fr_mem25, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %11)
  %mul_hi_c = fmul double %div_hi_abcd_max, -6.000000e+00
  %force_round_fr_mem27 = alloca double, align 8
  %12 = bitcast double* %force_round_fr_mem27 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %12)
  store volatile double %mul_hi_c, double* %force_round_fr_mem27, align 8
  %force_roundfr_rounded28 = load volatile double* %force_round_fr_mem27, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %12)
  %force_round_fr_mem29 = alloca double, align 8
  %13 = bitcast double* %force_round_fr_mem29 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %13)
  store volatile double %mul_hi_c, double* %force_round_fr_mem29, align 8
  %force_roundfr_rounded30 = load volatile double* %force_round_fr_mem29, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %13)
  %mul_hi_ab_test = fcmp olt double %force_roundfr_rounded24, %force_roundfr_rounded26
  %mul_hi_cd_test = fcmp olt double %force_roundfr_rounded28, %force_roundfr_rounded30
  %mul_hi_ab_max = select i1 %mul_hi_ab_test, double %force_roundfr_rounded26, double %force_roundfr_rounded24
  %mul_hi_cd_max = select i1 %mul_hi_cd_test, double %force_roundfr_rounded30, double %force_roundfr_rounded28
  %mul_hi_abcd_test = fcmp olt double %mul_hi_ab_max, %mul_hi_cd_max
  %mul_hi_abcd_max = select i1 %mul_hi_abcd_test, double %mul_hi_cd_max, double %mul_hi_ab_max
  call void @set_rounding_mode_down()
  %force_round_fr_mem31 = alloca double, align 8
  %14 = bitcast double* %force_round_fr_mem31 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %14)
  store volatile double %mul_hi_a, double* %force_round_fr_mem31, align 8
  %force_roundfr_rounded32 = load volatile double* %force_round_fr_mem31, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %14)
  %force_round_fr_mem33 = alloca double, align 8
  %15 = bitcast double* %force_round_fr_mem33 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %15)
  store volatile double %mul_hi_a, double* %force_round_fr_mem33, align 8
  %force_roundfr_rounded34 = load volatile double* %force_round_fr_mem33, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %15)
  %force_round_fr_mem35 = alloca double, align 8
  %16 = bitcast double* %force_round_fr_mem35 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %16)
  store volatile double %mul_hi_c, double* %force_round_fr_mem35, align 8
  %force_roundfr_rounded36 = load volatile double* %force_round_fr_mem35, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %16)
  %force_round_fr_mem37 = alloca double, align 8
  %17 = bitcast double* %force_round_fr_mem37 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %17)
  store volatile double %mul_hi_c, double* %force_round_fr_mem37, align 8
  %force_roundfr_rounded38 = load volatile double* %force_round_fr_mem37, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %17)
  %mul_lo_ab_test = fcmp olt double %force_roundfr_rounded34, %force_roundfr_rounded32
  %mul_lo_cd_test = fcmp olt double %force_roundfr_rounded38, %force_roundfr_rounded36
  %mul_lo_cd_min = select i1 %mul_lo_cd_test, double %force_roundfr_rounded38, double %force_roundfr_rounded36
  %mul_lo_ab_min = select i1 %mul_lo_ab_test, double %force_roundfr_rounded34, double %force_roundfr_rounded32
  %mul_lo_abcd_test = fcmp olt double %mul_lo_cd_min, %mul_lo_ab_min
  %mul_lo_abcd_min = select i1 %mul_lo_abcd_test, double %mul_lo_cd_min, double %mul_lo_ab_min
  %neg_lo39 = fsub double -0.000000e+00, %mul_hi_abcd_max
  %neg_hi40 = fsub double -0.000000e+00, %mul_lo_abcd_min
  call void @set_rounding_mode_up()
  %div_hi_a41 = fdiv double %c_lo, %neg_lo39
  %force_round_fr_mem42 = alloca double, align 8
  %18 = bitcast double* %force_round_fr_mem42 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %18)
  store volatile double %div_hi_a41, double* %force_round_fr_mem42, align 8
  %force_roundfr_rounded43 = load volatile double* %force_round_fr_mem42, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %18)
  %div_hi_b44 = fdiv double %c_lo, %neg_hi40
  %force_round_fr_mem45 = alloca double, align 8
  %19 = bitcast double* %force_round_fr_mem45 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %19)
  store volatile double %div_hi_b44, double* %force_round_fr_mem45, align 8
  %force_roundfr_rounded46 = load volatile double* %force_round_fr_mem45, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %19)
  %div_hi_c47 = fdiv double %c_hi, %neg_lo39
  %force_round_fr_mem48 = alloca double, align 8
  %20 = bitcast double* %force_round_fr_mem48 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %20)
  store volatile double %div_hi_c47, double* %force_round_fr_mem48, align 8
  %force_roundfr_rounded49 = load volatile double* %force_round_fr_mem48, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %20)
  %div_hi_d50 = fdiv double %c_hi, %neg_hi40
  %force_round_fr_mem51 = alloca double, align 8
  %21 = bitcast double* %force_round_fr_mem51 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %21)
  store volatile double %div_hi_d50, double* %force_round_fr_mem51, align 8
  %force_roundfr_rounded52 = load volatile double* %force_round_fr_mem51, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %21)
  %div_hi_ab_test53 = fcmp olt double %force_roundfr_rounded43, %force_roundfr_rounded46
  %div_hi_cd_test54 = fcmp olt double %force_roundfr_rounded49, %force_roundfr_rounded52
  %div_hi_ab_max55 = select i1 %div_hi_ab_test53, double %force_roundfr_rounded46, double %force_roundfr_rounded43
  %div_hi_cd_max56 = select i1 %div_hi_cd_test54, double %force_roundfr_rounded52, double %force_roundfr_rounded49
  %div_hi_abcd_test57 = fcmp olt double %div_hi_ab_max55, %div_hi_cd_max56
  %div_hi_abcd_max58 = select i1 %div_hi_abcd_test57, double %div_hi_cd_max56, double %div_hi_ab_max55
  call void @set_rounding_mode_down()
  %force_round_fr_mem60 = alloca double, align 8
  %22 = bitcast double* %force_round_fr_mem60 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %22)
  store volatile double %div_hi_a41, double* %force_round_fr_mem60, align 8
  %force_roundfr_rounded61 = load volatile double* %force_round_fr_mem60, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %22)
  %force_round_fr_mem63 = alloca double, align 8
  %23 = bitcast double* %force_round_fr_mem63 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %23)
  store volatile double %div_hi_b44, double* %force_round_fr_mem63, align 8
  %force_roundfr_rounded64 = load volatile double* %force_round_fr_mem63, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %23)
  %force_round_fr_mem66 = alloca double, align 8
  %24 = bitcast double* %force_round_fr_mem66 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %24)
  store volatile double %div_hi_c47, double* %force_round_fr_mem66, align 8
  %force_roundfr_rounded67 = load volatile double* %force_round_fr_mem66, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %24)
  %force_round_fr_mem69 = alloca double, align 8
  %25 = bitcast double* %force_round_fr_mem69 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %25)
  store volatile double %div_hi_d50, double* %force_round_fr_mem69, align 8
  %force_roundfr_rounded70 = load volatile double* %force_round_fr_mem69, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %25)
  %div_lo_ab_test71 = fcmp olt double %force_roundfr_rounded64, %force_roundfr_rounded61
  %div_lo_cd_test72 = fcmp olt double %force_roundfr_rounded70, %force_roundfr_rounded67
  %div_lo_cd_min73 = select i1 %div_lo_cd_test72, double %force_roundfr_rounded70, double %force_roundfr_rounded67
  %div_lo_ab_min74 = select i1 %div_lo_ab_test71, double %force_roundfr_rounded64, double %force_roundfr_rounded61
  %div_lo_abcd_test75 = fcmp olt double %div_lo_cd_min73, %div_lo_ab_min74
  %div_lo_abcd_min76 = select i1 %div_lo_abcd_test75, double %div_lo_cd_min73, double %div_lo_ab_min74
  call void @set_rounding_mode_up()
  %div_hi_a77 = fdiv double -6.600000e+01, %div_lo_abcd_min76
  %force_round_fr_mem78 = alloca double, align 8
  %26 = bitcast double* %force_round_fr_mem78 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %26)
  store volatile double %div_hi_a77, double* %force_round_fr_mem78, align 8
  %force_roundfr_rounded79 = load volatile double* %force_round_fr_mem78, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %26)
  %div_hi_b80 = fdiv double -6.600000e+01, %div_hi_abcd_max58
  %force_round_fr_mem81 = alloca double, align 8
  %27 = bitcast double* %force_round_fr_mem81 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %27)
  store volatile double %div_hi_b80, double* %force_round_fr_mem81, align 8
  %force_roundfr_rounded82 = load volatile double* %force_round_fr_mem81, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %27)
  %force_round_fr_mem84 = alloca double, align 8
  %28 = bitcast double* %force_round_fr_mem84 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %28)
  store volatile double %div_hi_a77, double* %force_round_fr_mem84, align 8
  %force_roundfr_rounded85 = load volatile double* %force_round_fr_mem84, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %28)
  %force_round_fr_mem87 = alloca double, align 8
  %29 = bitcast double* %force_round_fr_mem87 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %29)
  store volatile double %div_hi_b80, double* %force_round_fr_mem87, align 8
  %force_roundfr_rounded88 = load volatile double* %force_round_fr_mem87, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %29)
  %div_hi_ab_test89 = fcmp olt double %force_roundfr_rounded79, %force_roundfr_rounded82
  %div_hi_cd_test90 = fcmp olt double %force_roundfr_rounded85, %force_roundfr_rounded88
  %div_hi_ab_max91 = select i1 %div_hi_ab_test89, double %force_roundfr_rounded82, double %force_roundfr_rounded79
  %div_hi_cd_max92 = select i1 %div_hi_cd_test90, double %force_roundfr_rounded88, double %force_roundfr_rounded85
  %div_hi_abcd_test93 = fcmp olt double %div_hi_ab_max91, %div_hi_cd_max92
  %div_hi_abcd_max94 = select i1 %div_hi_abcd_test93, double %div_hi_cd_max92, double %div_hi_ab_max91
  call void @set_rounding_mode_down()
  %force_round_fr_mem96 = alloca double, align 8
  %30 = bitcast double* %force_round_fr_mem96 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %30)
  store volatile double %div_hi_a77, double* %force_round_fr_mem96, align 8
  %force_roundfr_rounded97 = load volatile double* %force_round_fr_mem96, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %30)
  %force_round_fr_mem99 = alloca double, align 8
  %31 = bitcast double* %force_round_fr_mem99 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %31)
  store volatile double %div_hi_b80, double* %force_round_fr_mem99, align 8
  %force_roundfr_rounded100 = load volatile double* %force_round_fr_mem99, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %31)
  %force_round_fr_mem102 = alloca double, align 8
  %32 = bitcast double* %force_round_fr_mem102 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %32)
  store volatile double %div_hi_a77, double* %force_round_fr_mem102, align 8
  %force_roundfr_rounded103 = load volatile double* %force_round_fr_mem102, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %32)
  %force_round_fr_mem105 = alloca double, align 8
  %33 = bitcast double* %force_round_fr_mem105 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %33)
  store volatile double %div_hi_b80, double* %force_round_fr_mem105, align 8
  %force_roundfr_rounded106 = load volatile double* %force_round_fr_mem105, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %33)
  %div_lo_ab_test107 = fcmp olt double %force_roundfr_rounded100, %force_roundfr_rounded97
  %div_lo_cd_test108 = fcmp olt double %force_roundfr_rounded106, %force_roundfr_rounded103
  %div_lo_cd_min109 = select i1 %div_lo_cd_test108, double %force_roundfr_rounded106, double %force_roundfr_rounded103
  %div_lo_ab_min110 = select i1 %div_lo_ab_test107, double %force_roundfr_rounded100, double %force_roundfr_rounded97
  %div_lo_abcd_test111 = fcmp olt double %div_lo_cd_min109, %div_lo_ab_min110
  %div_lo_abcd_min112 = select i1 %div_lo_abcd_test111, double %div_lo_cd_min109, double %div_lo_ab_min110
  %neg_lo113 = fsub double -0.000000e+00, %div_hi_abcd_max94
  %neg_hi114 = fsub double -0.000000e+00, %div_lo_abcd_min112
  call void @set_rounding_mode_up()
  %div_hi_a115 = fdiv double %neg_lo113, %d_lo
  %force_round_fr_mem116 = alloca double, align 8
  %34 = bitcast double* %force_round_fr_mem116 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %34)
  store volatile double %div_hi_a115, double* %force_round_fr_mem116, align 8
  %force_roundfr_rounded117 = load volatile double* %force_round_fr_mem116, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %34)
  %div_hi_b118 = fdiv double %neg_lo113, %d_hi
  %force_round_fr_mem119 = alloca double, align 8
  %35 = bitcast double* %force_round_fr_mem119 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %35)
  store volatile double %div_hi_b118, double* %force_round_fr_mem119, align 8
  %force_roundfr_rounded120 = load volatile double* %force_round_fr_mem119, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %35)
  %div_hi_c121 = fdiv double %neg_hi114, %d_lo
  %force_round_fr_mem122 = alloca double, align 8
  %36 = bitcast double* %force_round_fr_mem122 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %36)
  store volatile double %div_hi_c121, double* %force_round_fr_mem122, align 8
  %force_roundfr_rounded123 = load volatile double* %force_round_fr_mem122, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %36)
  %div_hi_d124 = fdiv double %neg_hi114, %d_hi
  %force_round_fr_mem125 = alloca double, align 8
  %37 = bitcast double* %force_round_fr_mem125 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %37)
  store volatile double %div_hi_d124, double* %force_round_fr_mem125, align 8
  %force_roundfr_rounded126 = load volatile double* %force_round_fr_mem125, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %37)
  %div_hi_ab_test127 = fcmp olt double %force_roundfr_rounded117, %force_roundfr_rounded120
  %div_hi_cd_test128 = fcmp olt double %force_roundfr_rounded123, %force_roundfr_rounded126
  %div_hi_ab_max129 = select i1 %div_hi_ab_test127, double %force_roundfr_rounded120, double %force_roundfr_rounded117
  %div_hi_cd_max130 = select i1 %div_hi_cd_test128, double %force_roundfr_rounded126, double %force_roundfr_rounded123
  %div_hi_abcd_test131 = fcmp olt double %div_hi_ab_max129, %div_hi_cd_max130
  %div_hi_abcd_max132 = select i1 %div_hi_abcd_test131, double %div_hi_cd_max130, double %div_hi_ab_max129
  call void @set_rounding_mode_down()
  %force_round_fr_mem134 = alloca double, align 8
  %38 = bitcast double* %force_round_fr_mem134 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %38)
  store volatile double %div_hi_a115, double* %force_round_fr_mem134, align 8
  %force_roundfr_rounded135 = load volatile double* %force_round_fr_mem134, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %38)
  %force_round_fr_mem137 = alloca double, align 8
  %39 = bitcast double* %force_round_fr_mem137 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %39)
  store volatile double %div_hi_b118, double* %force_round_fr_mem137, align 8
  %force_roundfr_rounded138 = load volatile double* %force_round_fr_mem137, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %39)
  %force_round_fr_mem140 = alloca double, align 8
  %40 = bitcast double* %force_round_fr_mem140 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %40)
  store volatile double %div_hi_c121, double* %force_round_fr_mem140, align 8
  %force_roundfr_rounded141 = load volatile double* %force_round_fr_mem140, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %40)
  %force_round_fr_mem143 = alloca double, align 8
  %41 = bitcast double* %force_round_fr_mem143 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %41)
  store volatile double %div_hi_d124, double* %force_round_fr_mem143, align 8
  %force_roundfr_rounded144 = load volatile double* %force_round_fr_mem143, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %41)
  %div_lo_ab_test145 = fcmp olt double %force_roundfr_rounded138, %force_roundfr_rounded135
  %div_lo_cd_test146 = fcmp olt double %force_roundfr_rounded144, %force_roundfr_rounded141
  %div_lo_cd_min147 = select i1 %div_lo_cd_test146, double %force_roundfr_rounded144, double %force_roundfr_rounded141
  %div_lo_ab_min148 = select i1 %div_lo_ab_test145, double %force_roundfr_rounded138, double %force_roundfr_rounded135
  %div_lo_abcd_test149 = fcmp olt double %div_lo_cd_min147, %div_lo_ab_min148
  %div_lo_abcd_min150 = select i1 %div_lo_abcd_test149, double %div_lo_cd_min147, double %div_lo_ab_min148
  call void @set_rounding_mode_up()
  %mul_hi_a151 = fmul double %div_lo_abcd_min150, %e_lo
  %force_round_fr_mem152 = alloca double, align 8
  %42 = bitcast double* %force_round_fr_mem152 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %42)
  store volatile double %mul_hi_a151, double* %force_round_fr_mem152, align 8
  %force_roundfr_rounded153 = load volatile double* %force_round_fr_mem152, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %42)
  %mul_hi_b154 = fmul double %div_lo_abcd_min150, %e_hi
  %force_round_fr_mem155 = alloca double, align 8
  %43 = bitcast double* %force_round_fr_mem155 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %43)
  store volatile double %mul_hi_b154, double* %force_round_fr_mem155, align 8
  %force_roundfr_rounded156 = load volatile double* %force_round_fr_mem155, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %43)
  %mul_hi_c157 = fmul double %div_hi_abcd_max132, %e_lo
  %force_round_fr_mem158 = alloca double, align 8
  %44 = bitcast double* %force_round_fr_mem158 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %44)
  store volatile double %mul_hi_c157, double* %force_round_fr_mem158, align 8
  %force_roundfr_rounded159 = load volatile double* %force_round_fr_mem158, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %44)
  %mul_hi_d160 = fmul double %div_hi_abcd_max132, %e_hi
  %force_round_fr_mem161 = alloca double, align 8
  %45 = bitcast double* %force_round_fr_mem161 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %45)
  store volatile double %mul_hi_d160, double* %force_round_fr_mem161, align 8
  %force_roundfr_rounded162 = load volatile double* %force_round_fr_mem161, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %45)
  %mul_hi_ab_test163 = fcmp olt double %force_roundfr_rounded153, %force_roundfr_rounded156
  %mul_hi_cd_test164 = fcmp olt double %force_roundfr_rounded159, %force_roundfr_rounded162
  %mul_hi_ab_max165 = select i1 %mul_hi_ab_test163, double %force_roundfr_rounded156, double %force_roundfr_rounded153
  %mul_hi_cd_max166 = select i1 %mul_hi_cd_test164, double %force_roundfr_rounded162, double %force_roundfr_rounded159
  %mul_hi_abcd_test167 = fcmp olt double %mul_hi_ab_max165, %mul_hi_cd_max166
  %mul_hi_abcd_max168 = select i1 %mul_hi_abcd_test167, double %mul_hi_cd_max166, double %mul_hi_ab_max165
  call void @set_rounding_mode_down()
  %force_round_fr_mem170 = alloca double, align 8
  %46 = bitcast double* %force_round_fr_mem170 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %46)
  store volatile double %mul_hi_a151, double* %force_round_fr_mem170, align 8
  %force_roundfr_rounded171 = load volatile double* %force_round_fr_mem170, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %46)
  %force_round_fr_mem173 = alloca double, align 8
  %47 = bitcast double* %force_round_fr_mem173 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %47)
  store volatile double %mul_hi_b154, double* %force_round_fr_mem173, align 8
  %force_roundfr_rounded174 = load volatile double* %force_round_fr_mem173, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %47)
  %force_round_fr_mem176 = alloca double, align 8
  %48 = bitcast double* %force_round_fr_mem176 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %48)
  store volatile double %mul_hi_c157, double* %force_round_fr_mem176, align 8
  %force_roundfr_rounded177 = load volatile double* %force_round_fr_mem176, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %48)
  %force_round_fr_mem179 = alloca double, align 8
  %49 = bitcast double* %force_round_fr_mem179 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %49)
  store volatile double %mul_hi_d160, double* %force_round_fr_mem179, align 8
  %force_roundfr_rounded180 = load volatile double* %force_round_fr_mem179, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %49)
  %mul_lo_ab_test181 = fcmp olt double %force_roundfr_rounded174, %force_roundfr_rounded171
  %mul_lo_cd_test182 = fcmp olt double %force_roundfr_rounded180, %force_roundfr_rounded177
  %mul_lo_cd_min183 = select i1 %mul_lo_cd_test182, double %force_roundfr_rounded180, double %force_roundfr_rounded177
  %mul_lo_ab_min184 = select i1 %mul_lo_ab_test181, double %force_roundfr_rounded174, double %force_roundfr_rounded171
  %mul_lo_abcd_test185 = fcmp olt double %mul_lo_cd_min183, %mul_lo_ab_min184
  %mul_lo_abcd_min186 = select i1 %mul_lo_abcd_test185, double %mul_lo_cd_min183, double %mul_lo_ab_min184
  %expt_odd_lo_unroll_x = fmul double %mul_lo_abcd_min186, %mul_lo_abcd_min186
  %force_round_fr_mem187 = alloca double, align 8
  %50 = bitcast double* %force_round_fr_mem187 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %50)
  store volatile double %expt_odd_lo_unroll_x, double* %force_round_fr_mem187, align 8
  %force_roundfr_rounded188 = load volatile double* %force_round_fr_mem187, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %50)
  %expt_odd_lopwr_do_y = fmul double %force_roundfr_rounded188, %mul_lo_abcd_min186
  %force_round_fr_mem189 = alloca double, align 8
  %51 = bitcast double* %force_round_fr_mem189 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %51)
  store volatile double %expt_odd_lopwr_do_y, double* %force_round_fr_mem189, align 8
  %force_roundfr_rounded190 = load volatile double* %force_round_fr_mem189, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %51)
  call void @set_rounding_mode_up()
  %expt_odd_hi_unroll_x = fmul double %mul_hi_abcd_max168, %mul_hi_abcd_max168
  %force_round_fr_mem191 = alloca double, align 8
  %52 = bitcast double* %force_round_fr_mem191 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %52)
  store volatile double %expt_odd_hi_unroll_x, double* %force_round_fr_mem191, align 8
  %force_roundfr_rounded192 = load volatile double* %force_round_fr_mem191, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %52)
  %expt_odd_hipwr_do_y = fmul double %force_roundfr_rounded192, %mul_hi_abcd_max168
  %force_round_fr_mem193 = alloca double, align 8
  %53 = bitcast double* %force_round_fr_mem193 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %53)
  store volatile double %expt_odd_hipwr_do_y, double* %force_round_fr_mem193, align 8
  %force_roundfr_rounded194 = load volatile double* %force_round_fr_mem193, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %53)
  %expt_even_are_both_pos_p = fcmp ogt double %force_roundfr_rounded190, 0.000000e+00
  br i1 %expt_even_are_both_pos_p, label %expt_even_both_are_pos, label %expt_even_both_are_not_pos

expt_even_both_are_pos:                           ; preds = %entry
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x = fmul double %force_roundfr_rounded190, %force_roundfr_rounded190
  %force_round_fr_mem195 = alloca double, align 8
  %54 = bitcast double* %force_round_fr_mem195 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %54)
  store volatile double %expt_even_pos_block_res_lo_unroll_x, double* %force_round_fr_mem195, align 8
  %force_roundfr_rounded196 = load volatile double* %force_round_fr_mem195, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %54)
  %force_round_fr_mem197 = alloca double, align 8
  %55 = bitcast double* %force_round_fr_mem197 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %55)
  store volatile double %force_roundfr_rounded196, double* %force_round_fr_mem197, align 8
  %force_roundfr_rounded198 = load volatile double* %force_round_fr_mem197, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %55)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x = fmul double %force_roundfr_rounded194, %force_roundfr_rounded194
  %force_round_fr_mem199 = alloca double, align 8
  %56 = bitcast double* %force_round_fr_mem199 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %56)
  store volatile double %expt_even_pos_block_res_hi_unroll_x, double* %force_round_fr_mem199, align 8
  %force_roundfr_rounded200 = load volatile double* %force_round_fr_mem199, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %56)
  %force_round_fr_mem201 = alloca double, align 8
  %57 = bitcast double* %force_round_fr_mem201 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %57)
  store volatile double %force_roundfr_rounded200, double* %force_round_fr_mem201, align 8
  %force_roundfr_rounded202 = load volatile double* %force_round_fr_mem201, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %57)
  br label %expt_even_merge

expt_even_both_are_not_pos:                       ; preds = %entry
  %expt_even_are_both_neg_p = fcmp olt double %force_roundfr_rounded194, 0.000000e+00
  br i1 %expt_even_are_both_neg_p, label %expt_even_both_are_neg, label %expt_even_contains_zero

expt_even_both_are_neg:                           ; preds = %expt_even_both_are_not_pos
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x = fmul double %force_roundfr_rounded194, %force_roundfr_rounded194
  %force_round_fr_mem203 = alloca double, align 8
  %58 = bitcast double* %force_round_fr_mem203 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %58)
  store volatile double %expt_even_neg_block_res_lo_unroll_x, double* %force_round_fr_mem203, align 8
  %force_roundfr_rounded204 = load volatile double* %force_round_fr_mem203, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %58)
  %force_round_fr_mem205 = alloca double, align 8
  %59 = bitcast double* %force_round_fr_mem205 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %59)
  store volatile double %force_roundfr_rounded204, double* %force_round_fr_mem205, align 8
  %force_roundfr_rounded206 = load volatile double* %force_round_fr_mem205, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %59)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x207 = fmul double %force_roundfr_rounded190, %force_roundfr_rounded190
  %force_round_fr_mem208 = alloca double, align 8
  %60 = bitcast double* %force_round_fr_mem208 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %60)
  store volatile double %expt_even_pos_block_res_hi_unroll_x207, double* %force_round_fr_mem208, align 8
  %force_roundfr_rounded209 = load volatile double* %force_round_fr_mem208, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %60)
  %force_round_fr_mem211 = alloca double, align 8
  %61 = bitcast double* %force_round_fr_mem211 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %61)
  store volatile double %force_roundfr_rounded209, double* %force_round_fr_mem211, align 8
  %force_roundfr_rounded212 = load volatile double* %force_round_fr_mem211, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %61)
  br label %expt_even_merge

expt_even_contains_zero:                          ; preds = %expt_even_both_are_not_pos
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x = fmul double %force_roundfr_rounded190, %force_roundfr_rounded190
  %force_round_fr_mem213 = alloca double, align 8
  %62 = bitcast double* %force_round_fr_mem213 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %62)
  store volatile double %expt_even_mid_block_lo__unroll_x, double* %force_round_fr_mem213, align 8
  %force_roundfr_rounded214 = load volatile double* %force_round_fr_mem213, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %62)
  %force_round_fr_mem215 = alloca double, align 8
  %63 = bitcast double* %force_round_fr_mem215 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %63)
  store volatile double %force_roundfr_rounded214, double* %force_round_fr_mem215, align 8
  %force_roundfr_rounded216 = load volatile double* %force_round_fr_mem215, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %63)
  %expt_even_mid_block_hi__unroll_x = fmul double %force_roundfr_rounded194, %force_roundfr_rounded194
  %force_round_fr_mem217 = alloca double, align 8
  %64 = bitcast double* %force_round_fr_mem217 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %64)
  store volatile double %expt_even_mid_block_hi__unroll_x, double* %force_round_fr_mem217, align 8
  %force_roundfr_rounded218 = load volatile double* %force_round_fr_mem217, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %64)
  %force_round_fr_mem219 = alloca double, align 8
  %65 = bitcast double* %force_round_fr_mem219 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %65)
  store volatile double %force_roundfr_rounded218, double* %force_round_fr_mem219, align 8
  %force_roundfr_rounded220 = load volatile double* %force_round_fr_mem219, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %65)
  %expt_even_mid_block_res_hi_abtest = fcmp olt double %force_roundfr_rounded216, %force_roundfr_rounded220
  %expt_even_mid_block_res_hi_abmax = select i1 %expt_even_mid_block_res_hi_abtest, double %force_roundfr_rounded220, double %force_roundfr_rounded216
  br label %expt_even_merge

expt_even_merge:                                  ; preds = %expt_even_contains_zero, %expt_even_both_are_neg, %expt_even_both_are_pos
  %expt_lo = phi double [ %force_roundfr_rounded198, %expt_even_both_are_pos ], [ %force_roundfr_rounded206, %expt_even_both_are_neg ], [ 0.000000e+00, %expt_even_contains_zero ]
  %expt_hi = phi double [ %force_roundfr_rounded202, %expt_even_both_are_pos ], [ %force_roundfr_rounded212, %expt_even_both_are_neg ], [ %expt_even_mid_block_res_hi_abmax, %expt_even_contains_zero ]
  %mul_hi_a221 = fmul double %expt_lo, -3.400000e+01
  %force_round_fr_mem222 = alloca double, align 8
  %66 = bitcast double* %force_round_fr_mem222 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %66)
  store volatile double %mul_hi_a221, double* %force_round_fr_mem222, align 8
  %force_roundfr_rounded223 = load volatile double* %force_round_fr_mem222, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %66)
  %mul_hi_b224 = fmul double %expt_hi, -3.400000e+01
  %force_round_fr_mem225 = alloca double, align 8
  %67 = bitcast double* %force_round_fr_mem225 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %67)
  store volatile double %mul_hi_b224, double* %force_round_fr_mem225, align 8
  %force_roundfr_rounded226 = load volatile double* %force_round_fr_mem225, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %67)
  %force_round_fr_mem228 = alloca double, align 8
  %68 = bitcast double* %force_round_fr_mem228 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %68)
  store volatile double %mul_hi_a221, double* %force_round_fr_mem228, align 8
  %force_roundfr_rounded229 = load volatile double* %force_round_fr_mem228, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %68)
  %force_round_fr_mem231 = alloca double, align 8
  %69 = bitcast double* %force_round_fr_mem231 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %69)
  store volatile double %mul_hi_b224, double* %force_round_fr_mem231, align 8
  %force_roundfr_rounded232 = load volatile double* %force_round_fr_mem231, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %69)
  %mul_hi_ab_test233 = fcmp olt double %force_roundfr_rounded223, %force_roundfr_rounded226
  %mul_hi_cd_test234 = fcmp olt double %force_roundfr_rounded229, %force_roundfr_rounded232
  %mul_hi_ab_max235 = select i1 %mul_hi_ab_test233, double %force_roundfr_rounded226, double %force_roundfr_rounded223
  %mul_hi_cd_max236 = select i1 %mul_hi_cd_test234, double %force_roundfr_rounded232, double %force_roundfr_rounded229
  %mul_hi_abcd_test237 = fcmp olt double %mul_hi_ab_max235, %mul_hi_cd_max236
  %mul_hi_abcd_max238 = select i1 %mul_hi_abcd_test237, double %mul_hi_cd_max236, double %mul_hi_ab_max235
  call void @set_rounding_mode_down()
  %force_round_fr_mem240 = alloca double, align 8
  %70 = bitcast double* %force_round_fr_mem240 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %70)
  store volatile double %mul_hi_a221, double* %force_round_fr_mem240, align 8
  %force_roundfr_rounded241 = load volatile double* %force_round_fr_mem240, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %70)
  %force_round_fr_mem243 = alloca double, align 8
  %71 = bitcast double* %force_round_fr_mem243 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %71)
  store volatile double %mul_hi_b224, double* %force_round_fr_mem243, align 8
  %force_roundfr_rounded244 = load volatile double* %force_round_fr_mem243, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %71)
  %force_round_fr_mem246 = alloca double, align 8
  %72 = bitcast double* %force_round_fr_mem246 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %72)
  store volatile double %mul_hi_a221, double* %force_round_fr_mem246, align 8
  %force_roundfr_rounded247 = load volatile double* %force_round_fr_mem246, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %72)
  %force_round_fr_mem249 = alloca double, align 8
  %73 = bitcast double* %force_round_fr_mem249 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %73)
  store volatile double %mul_hi_b224, double* %force_round_fr_mem249, align 8
  %force_roundfr_rounded250 = load volatile double* %force_round_fr_mem249, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %73)
  %mul_lo_ab_test251 = fcmp olt double %force_roundfr_rounded244, %force_roundfr_rounded241
  %mul_lo_cd_test252 = fcmp olt double %force_roundfr_rounded250, %force_roundfr_rounded247
  %mul_lo_cd_min253 = select i1 %mul_lo_cd_test252, double %force_roundfr_rounded250, double %force_roundfr_rounded247
  %mul_lo_ab_min254 = select i1 %mul_lo_ab_test251, double %force_roundfr_rounded244, double %force_roundfr_rounded241
  %mul_lo_abcd_test255 = fcmp olt double %mul_lo_cd_min253, %mul_lo_ab_min254
  %mul_lo_abcd_min256 = select i1 %mul_lo_abcd_test255, double %mul_lo_cd_min253, double %mul_lo_ab_min254
  %add_lo259 = fadd double %mul_lo_abcd_min256, -7.100000e+01
  %force_round_fr_mem260 = alloca double, align 8
  %74 = bitcast double* %force_round_fr_mem260 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %74)
  store volatile double %add_lo259, double* %force_round_fr_mem260, align 8
  %force_roundfr_rounded261 = load volatile double* %force_round_fr_mem260, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %74)
  call void @set_rounding_mode_up()
  %add_hi262 = fadd double %mul_hi_abcd_max238, -7.100000e+01
  %force_round_fr_mem263 = alloca double, align 8
  %75 = bitcast double* %force_round_fr_mem263 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %75)
  store volatile double %add_hi262, double* %force_round_fr_mem263, align 8
  %force_roundfr_rounded264 = load volatile double* %force_round_fr_mem263, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %75)
  call void @set_rounding_mode_down()
  %add_lo265 = fadd double %force_roundfr_rounded261, 4.900000e+01
  %force_round_fr_mem266 = alloca double, align 8
  %76 = bitcast double* %force_round_fr_mem266 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %76)
  store volatile double %add_lo265, double* %force_round_fr_mem266, align 8
  %force_roundfr_rounded267 = load volatile double* %force_round_fr_mem266, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %76)
  call void @set_rounding_mode_up()
  %add_hi268 = fadd double %force_roundfr_rounded264, 4.900000e+01
  %force_round_fr_mem269 = alloca double, align 8
  %77 = bitcast double* %force_round_fr_mem269 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %77)
  store volatile double %add_hi268, double* %force_round_fr_mem269, align 8
  %force_roundfr_rounded270 = load volatile double* %force_round_fr_mem269, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %77)
  call void @set_rounding_mode_down()
  %sub_lo = fsub double %force_roundfr_rounded267, %f_hi
  %force_round_fr_mem271 = alloca double, align 8
  %78 = bitcast double* %force_round_fr_mem271 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %78)
  store volatile double %sub_lo, double* %force_round_fr_mem271, align 8
  %force_roundfr_rounded272 = load volatile double* %force_round_fr_mem271, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %78)
  call void @set_rounding_mode_up()
  %sub_hi = fsub double %force_roundfr_rounded270, %f_lo
  %force_round_fr_mem273 = alloca double, align 8
  %79 = bitcast double* %force_round_fr_mem273 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %79)
  store volatile double %sub_hi, double* %force_round_fr_mem273, align 8
  %force_roundfr_rounded274 = load volatile double* %force_round_fr_mem273, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %79)
  %neg_lo275 = fsub double -0.000000e+00, %force_roundfr_rounded274
  %neg_hi276 = fsub double -0.000000e+00, %force_roundfr_rounded272
  %mul_hi_a277 = fmul double %neg_lo275, %g_lo
  %force_round_fr_mem278 = alloca double, align 8
  %80 = bitcast double* %force_round_fr_mem278 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %80)
  store volatile double %mul_hi_a277, double* %force_round_fr_mem278, align 8
  %force_roundfr_rounded279 = load volatile double* %force_round_fr_mem278, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %80)
  %mul_hi_b280 = fmul double %neg_lo275, %g_hi
  %force_round_fr_mem281 = alloca double, align 8
  %81 = bitcast double* %force_round_fr_mem281 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %81)
  store volatile double %mul_hi_b280, double* %force_round_fr_mem281, align 8
  %force_roundfr_rounded282 = load volatile double* %force_round_fr_mem281, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %81)
  %mul_hi_c283 = fmul double %neg_hi276, %g_lo
  %force_round_fr_mem284 = alloca double, align 8
  %82 = bitcast double* %force_round_fr_mem284 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %82)
  store volatile double %mul_hi_c283, double* %force_round_fr_mem284, align 8
  %force_roundfr_rounded285 = load volatile double* %force_round_fr_mem284, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %82)
  %mul_hi_d286 = fmul double %neg_hi276, %g_hi
  %force_round_fr_mem287 = alloca double, align 8
  %83 = bitcast double* %force_round_fr_mem287 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %83)
  store volatile double %mul_hi_d286, double* %force_round_fr_mem287, align 8
  %force_roundfr_rounded288 = load volatile double* %force_round_fr_mem287, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %83)
  %mul_hi_ab_test289 = fcmp olt double %force_roundfr_rounded279, %force_roundfr_rounded282
  %mul_hi_cd_test290 = fcmp olt double %force_roundfr_rounded285, %force_roundfr_rounded288
  %mul_hi_ab_max291 = select i1 %mul_hi_ab_test289, double %force_roundfr_rounded282, double %force_roundfr_rounded279
  %mul_hi_cd_max292 = select i1 %mul_hi_cd_test290, double %force_roundfr_rounded288, double %force_roundfr_rounded285
  %mul_hi_abcd_test293 = fcmp olt double %mul_hi_ab_max291, %mul_hi_cd_max292
  %mul_hi_abcd_max294 = select i1 %mul_hi_abcd_test293, double %mul_hi_cd_max292, double %mul_hi_ab_max291
  call void @set_rounding_mode_down()
  %force_round_fr_mem296 = alloca double, align 8
  %84 = bitcast double* %force_round_fr_mem296 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %84)
  store volatile double %mul_hi_a277, double* %force_round_fr_mem296, align 8
  %force_roundfr_rounded297 = load volatile double* %force_round_fr_mem296, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %84)
  %force_round_fr_mem299 = alloca double, align 8
  %85 = bitcast double* %force_round_fr_mem299 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %85)
  store volatile double %mul_hi_b280, double* %force_round_fr_mem299, align 8
  %force_roundfr_rounded300 = load volatile double* %force_round_fr_mem299, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %85)
  %force_round_fr_mem302 = alloca double, align 8
  %86 = bitcast double* %force_round_fr_mem302 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %86)
  store volatile double %mul_hi_c283, double* %force_round_fr_mem302, align 8
  %force_roundfr_rounded303 = load volatile double* %force_round_fr_mem302, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %86)
  %force_round_fr_mem305 = alloca double, align 8
  %87 = bitcast double* %force_round_fr_mem305 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %87)
  store volatile double %mul_hi_d286, double* %force_round_fr_mem305, align 8
  %force_roundfr_rounded306 = load volatile double* %force_round_fr_mem305, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %87)
  %mul_lo_ab_test307 = fcmp olt double %force_roundfr_rounded300, %force_roundfr_rounded297
  %mul_lo_cd_test308 = fcmp olt double %force_roundfr_rounded306, %force_roundfr_rounded303
  %mul_lo_cd_min309 = select i1 %mul_lo_cd_test308, double %force_roundfr_rounded306, double %force_roundfr_rounded303
  %mul_lo_ab_min310 = select i1 %mul_lo_ab_test307, double %force_roundfr_rounded300, double %force_roundfr_rounded297
  %mul_lo_abcd_test311 = fcmp olt double %mul_lo_cd_min309, %mul_lo_ab_min310
  %mul_lo_abcd_min312 = select i1 %mul_lo_abcd_test311, double %mul_lo_cd_min309, double %mul_lo_ab_min310
  %sub_lo313 = fsub double %h_lo, %mul_hi_abcd_max294
  %force_round_fr_mem314 = alloca double, align 8
  %88 = bitcast double* %force_round_fr_mem314 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %88)
  store volatile double %sub_lo313, double* %force_round_fr_mem314, align 8
  %force_roundfr_rounded315 = load volatile double* %force_round_fr_mem314, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %88)
  call void @set_rounding_mode_up()
  %sub_hi316 = fsub double %h_hi, %mul_lo_abcd_min312
  %force_round_fr_mem317 = alloca double, align 8
  %89 = bitcast double* %force_round_fr_mem317 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %89)
  store volatile double %sub_hi316, double* %force_round_fr_mem317, align 8
  %force_roundfr_rounded318 = load volatile double* %force_round_fr_mem317, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %89)
  %div_hi_a319 = fdiv double 1.600000e+01, %force_roundfr_rounded315
  %force_round_fr_mem320 = alloca double, align 8
  %90 = bitcast double* %force_round_fr_mem320 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %90)
  store volatile double %div_hi_a319, double* %force_round_fr_mem320, align 8
  %force_roundfr_rounded321 = load volatile double* %force_round_fr_mem320, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %90)
  %div_hi_b322 = fdiv double 1.600000e+01, %force_roundfr_rounded318
  %force_round_fr_mem323 = alloca double, align 8
  %91 = bitcast double* %force_round_fr_mem323 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %91)
  store volatile double %div_hi_b322, double* %force_round_fr_mem323, align 8
  %force_roundfr_rounded324 = load volatile double* %force_round_fr_mem323, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %91)
  %force_round_fr_mem326 = alloca double, align 8
  %92 = bitcast double* %force_round_fr_mem326 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %92)
  store volatile double %div_hi_a319, double* %force_round_fr_mem326, align 8
  %force_roundfr_rounded327 = load volatile double* %force_round_fr_mem326, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %92)
  %force_round_fr_mem329 = alloca double, align 8
  %93 = bitcast double* %force_round_fr_mem329 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %93)
  store volatile double %div_hi_b322, double* %force_round_fr_mem329, align 8
  %force_roundfr_rounded330 = load volatile double* %force_round_fr_mem329, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %93)
  %div_hi_ab_test331 = fcmp olt double %force_roundfr_rounded321, %force_roundfr_rounded324
  %div_hi_cd_test332 = fcmp olt double %force_roundfr_rounded327, %force_roundfr_rounded330
  %div_hi_ab_max333 = select i1 %div_hi_ab_test331, double %force_roundfr_rounded324, double %force_roundfr_rounded321
  %div_hi_cd_max334 = select i1 %div_hi_cd_test332, double %force_roundfr_rounded330, double %force_roundfr_rounded327
  %div_hi_abcd_test335 = fcmp olt double %div_hi_ab_max333, %div_hi_cd_max334
  %div_hi_abcd_max336 = select i1 %div_hi_abcd_test335, double %div_hi_cd_max334, double %div_hi_ab_max333
  call void @set_rounding_mode_down()
  %force_round_fr_mem338 = alloca double, align 8
  %94 = bitcast double* %force_round_fr_mem338 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %94)
  store volatile double %div_hi_a319, double* %force_round_fr_mem338, align 8
  %force_roundfr_rounded339 = load volatile double* %force_round_fr_mem338, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %94)
  %force_round_fr_mem341 = alloca double, align 8
  %95 = bitcast double* %force_round_fr_mem341 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %95)
  store volatile double %div_hi_b322, double* %force_round_fr_mem341, align 8
  %force_roundfr_rounded342 = load volatile double* %force_round_fr_mem341, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %95)
  %force_round_fr_mem344 = alloca double, align 8
  %96 = bitcast double* %force_round_fr_mem344 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %96)
  store volatile double %div_hi_a319, double* %force_round_fr_mem344, align 8
  %force_roundfr_rounded345 = load volatile double* %force_round_fr_mem344, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %96)
  %force_round_fr_mem347 = alloca double, align 8
  %97 = bitcast double* %force_round_fr_mem347 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %97)
  store volatile double %div_hi_b322, double* %force_round_fr_mem347, align 8
  %force_roundfr_rounded348 = load volatile double* %force_round_fr_mem347, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %97)
  %div_lo_ab_test349 = fcmp olt double %force_roundfr_rounded342, %force_roundfr_rounded339
  %div_lo_cd_test350 = fcmp olt double %force_roundfr_rounded348, %force_roundfr_rounded345
  %div_lo_cd_min351 = select i1 %div_lo_cd_test350, double %force_roundfr_rounded348, double %force_roundfr_rounded345
  %div_lo_ab_min352 = select i1 %div_lo_ab_test349, double %force_roundfr_rounded342, double %force_roundfr_rounded339
  %div_lo_abcd_test353 = fcmp olt double %div_lo_cd_min351, %div_lo_ab_min352
  %div_lo_abcd_min354 = select i1 %div_lo_abcd_test353, double %div_lo_cd_min351, double %div_lo_ab_min352
  %add_lo355 = fadd double %div_lo_abcd_min354, %i_lo
  %force_round_fr_mem356 = alloca double, align 8
  %98 = bitcast double* %force_round_fr_mem356 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %98)
  store volatile double %add_lo355, double* %force_round_fr_mem356, align 8
  %force_roundfr_rounded357 = load volatile double* %force_round_fr_mem356, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %98)
  call void @set_rounding_mode_up()
  %add_hi358 = fadd double %div_hi_abcd_max336, %i_hi
  %force_round_fr_mem359 = alloca double, align 8
  %99 = bitcast double* %force_round_fr_mem359 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %99)
  store volatile double %add_hi358, double* %force_round_fr_mem359, align 8
  %force_roundfr_rounded360 = load volatile double* %force_round_fr_mem359, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %99)
  %expt_even_are_both_pos_p361 = fcmp ogt double %force_roundfr_rounded357, 0.000000e+00
  br i1 %expt_even_are_both_pos_p361, label %expt_even_both_are_pos362, label %expt_even_both_are_not_pos381

expt_even_both_are_pos362:                        ; preds = %expt_even_merge
  call void @set_rounding_mode_down()
  %expt_even_pos_block_res_lo_unroll_x363 = fmul double %force_roundfr_rounded357, %force_roundfr_rounded357
  %force_round_fr_mem364 = alloca double, align 8
  %100 = bitcast double* %force_round_fr_mem364 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %100)
  store volatile double %expt_even_pos_block_res_lo_unroll_x363, double* %force_round_fr_mem364, align 8
  %force_roundfr_rounded365 = load volatile double* %force_round_fr_mem364, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %100)
  %expt_even_pos_block_res_lo_unroll_x366 = fmul double %force_roundfr_rounded365, %force_roundfr_rounded365
  %force_round_fr_mem367 = alloca double, align 8
  %101 = bitcast double* %force_round_fr_mem367 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %101)
  store volatile double %expt_even_pos_block_res_lo_unroll_x366, double* %force_round_fr_mem367, align 8
  %force_roundfr_rounded368 = load volatile double* %force_round_fr_mem367, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %101)
  %force_round_fr_mem370 = alloca double, align 8
  %102 = bitcast double* %force_round_fr_mem370 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %102)
  store volatile double %force_roundfr_rounded368, double* %force_round_fr_mem370, align 8
  %force_roundfr_rounded371 = load volatile double* %force_round_fr_mem370, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %102)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x372 = fmul double %force_roundfr_rounded360, %force_roundfr_rounded360
  %force_round_fr_mem373 = alloca double, align 8
  %103 = bitcast double* %force_round_fr_mem373 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %103)
  store volatile double %expt_even_pos_block_res_hi_unroll_x372, double* %force_round_fr_mem373, align 8
  %force_roundfr_rounded374 = load volatile double* %force_round_fr_mem373, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %103)
  %expt_even_pos_block_res_hi_unroll_x375 = fmul double %force_roundfr_rounded374, %force_roundfr_rounded374
  %force_round_fr_mem376 = alloca double, align 8
  %104 = bitcast double* %force_round_fr_mem376 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %104)
  store volatile double %expt_even_pos_block_res_hi_unroll_x375, double* %force_round_fr_mem376, align 8
  %force_roundfr_rounded377 = load volatile double* %force_round_fr_mem376, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %104)
  %force_round_fr_mem379 = alloca double, align 8
  %105 = bitcast double* %force_round_fr_mem379 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %105)
  store volatile double %force_roundfr_rounded377, double* %force_round_fr_mem379, align 8
  %force_roundfr_rounded380 = load volatile double* %force_round_fr_mem379, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %105)
  br label %expt_even_merge423

expt_even_both_are_not_pos381:                    ; preds = %expt_even_merge
  %expt_even_are_both_neg_p382 = fcmp olt double %force_roundfr_rounded360, 0.000000e+00
  br i1 %expt_even_are_both_neg_p382, label %expt_even_both_are_neg383, label %expt_even_contains_zero402

expt_even_both_are_neg383:                        ; preds = %expt_even_both_are_not_pos381
  call void @set_rounding_mode_down()
  %expt_even_neg_block_res_lo_unroll_x384 = fmul double %force_roundfr_rounded360, %force_roundfr_rounded360
  %force_round_fr_mem385 = alloca double, align 8
  %106 = bitcast double* %force_round_fr_mem385 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %106)
  store volatile double %expt_even_neg_block_res_lo_unroll_x384, double* %force_round_fr_mem385, align 8
  %force_roundfr_rounded386 = load volatile double* %force_round_fr_mem385, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %106)
  %expt_even_neg_block_res_lo_unroll_x387 = fmul double %force_roundfr_rounded386, %force_roundfr_rounded386
  %force_round_fr_mem388 = alloca double, align 8
  %107 = bitcast double* %force_round_fr_mem388 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %107)
  store volatile double %expt_even_neg_block_res_lo_unroll_x387, double* %force_round_fr_mem388, align 8
  %force_roundfr_rounded389 = load volatile double* %force_round_fr_mem388, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %107)
  %force_round_fr_mem391 = alloca double, align 8
  %108 = bitcast double* %force_round_fr_mem391 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %108)
  store volatile double %force_roundfr_rounded389, double* %force_round_fr_mem391, align 8
  %force_roundfr_rounded392 = load volatile double* %force_round_fr_mem391, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %108)
  call void @set_rounding_mode_up()
  %expt_even_pos_block_res_hi_unroll_x393 = fmul double %force_roundfr_rounded357, %force_roundfr_rounded357
  %force_round_fr_mem394 = alloca double, align 8
  %109 = bitcast double* %force_round_fr_mem394 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %109)
  store volatile double %expt_even_pos_block_res_hi_unroll_x393, double* %force_round_fr_mem394, align 8
  %force_roundfr_rounded395 = load volatile double* %force_round_fr_mem394, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %109)
  %expt_even_pos_block_res_hi_unroll_x396 = fmul double %force_roundfr_rounded395, %force_roundfr_rounded395
  %force_round_fr_mem397 = alloca double, align 8
  %110 = bitcast double* %force_round_fr_mem397 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %110)
  store volatile double %expt_even_pos_block_res_hi_unroll_x396, double* %force_round_fr_mem397, align 8
  %force_roundfr_rounded398 = load volatile double* %force_round_fr_mem397, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %110)
  %force_round_fr_mem400 = alloca double, align 8
  %111 = bitcast double* %force_round_fr_mem400 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %111)
  store volatile double %force_roundfr_rounded398, double* %force_round_fr_mem400, align 8
  %force_roundfr_rounded401 = load volatile double* %force_round_fr_mem400, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %111)
  br label %expt_even_merge423

expt_even_contains_zero402:                       ; preds = %expt_even_both_are_not_pos381
  call void @set_rounding_mode_up()
  %expt_even_mid_block_lo__unroll_x403 = fmul double %force_roundfr_rounded357, %force_roundfr_rounded357
  %force_round_fr_mem404 = alloca double, align 8
  %112 = bitcast double* %force_round_fr_mem404 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %112)
  store volatile double %expt_even_mid_block_lo__unroll_x403, double* %force_round_fr_mem404, align 8
  %force_roundfr_rounded405 = load volatile double* %force_round_fr_mem404, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %112)
  %expt_even_mid_block_lo__unroll_x406 = fmul double %force_roundfr_rounded405, %force_roundfr_rounded405
  %force_round_fr_mem407 = alloca double, align 8
  %113 = bitcast double* %force_round_fr_mem407 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %113)
  store volatile double %expt_even_mid_block_lo__unroll_x406, double* %force_round_fr_mem407, align 8
  %force_roundfr_rounded408 = load volatile double* %force_round_fr_mem407, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %113)
  %force_round_fr_mem410 = alloca double, align 8
  %114 = bitcast double* %force_round_fr_mem410 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %114)
  store volatile double %force_roundfr_rounded408, double* %force_round_fr_mem410, align 8
  %force_roundfr_rounded411 = load volatile double* %force_round_fr_mem410, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %114)
  %expt_even_mid_block_hi__unroll_x412 = fmul double %force_roundfr_rounded360, %force_roundfr_rounded360
  %force_round_fr_mem413 = alloca double, align 8
  %115 = bitcast double* %force_round_fr_mem413 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %115)
  store volatile double %expt_even_mid_block_hi__unroll_x412, double* %force_round_fr_mem413, align 8
  %force_roundfr_rounded414 = load volatile double* %force_round_fr_mem413, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %115)
  %expt_even_mid_block_hi__unroll_x415 = fmul double %force_roundfr_rounded414, %force_roundfr_rounded414
  %force_round_fr_mem416 = alloca double, align 8
  %116 = bitcast double* %force_round_fr_mem416 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %116)
  store volatile double %expt_even_mid_block_hi__unroll_x415, double* %force_round_fr_mem416, align 8
  %force_roundfr_rounded417 = load volatile double* %force_round_fr_mem416, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %116)
  %force_round_fr_mem419 = alloca double, align 8
  %117 = bitcast double* %force_round_fr_mem419 to i8*
  call void @llvm.lifetime.start(i64 -1, i8* %117)
  store volatile double %force_roundfr_rounded417, double* %force_round_fr_mem419, align 8
  %force_roundfr_rounded420 = load volatile double* %force_round_fr_mem419, align 8
  call void @llvm.lifetime.end(i64 -1, i8* %117)
  %expt_even_mid_block_res_hi_abtest421 = fcmp olt double %force_roundfr_rounded411, %force_roundfr_rounded420
  %expt_even_mid_block_res_hi_abmax422 = select i1 %expt_even_mid_block_res_hi_abtest421, double %force_roundfr_rounded420, double %force_roundfr_rounded411
  br label %expt_even_merge423

expt_even_merge423:                               ; preds = %expt_even_contains_zero402, %expt_even_both_are_neg383, %expt_even_both_are_pos362
  %expt_lo424 = phi double [ %force_roundfr_rounded371, %expt_even_both_are_pos362 ], [ %force_roundfr_rounded392, %expt_even_both_are_neg383 ], [ 0.000000e+00, %expt_even_contains_zero402 ]
  %expt_hi425 = phi double [ %force_roundfr_rounded380, %expt_even_both_are_pos362 ], [ %force_roundfr_rounded401, %expt_even_both_are_neg383 ], [ %expt_even_mid_block_res_hi_abmax422, %expt_even_contains_zero402 ]
  %func_res_interval_init = insertvalue { double, double } undef, double %expt_lo424, 0
  %func_res_interval = insertvalue { double, double } %func_res_interval_init, double %expt_hi425, 1
  ret { double, double } %func_res_interval
}