// RUN: %clang_cc1 -emit-llvm -triple=i386-pc-win32 -fms-compatibility %s -o - | FileCheck %s

enum Enum { zero, one, two };

struct __declspec(dllexport) S {
  // In MS compatibility mode, this counts as a definition.
  // Since it is exported, it must be emitted even if it's unreferenced.
  static const short x = 42;

  // This works for enums too.
  static const Enum y = two;

  struct NonExported {
    // dllexport is not inherited by this nested class.
    // Since z is not referenced, it should not be emitted.
    static const int z = 42;
  };
};

// CHECK: @"\01?x@S@@2FB" = weak_odr dllexport constant i16 42, align 2
// CHECK: @"\01?y@S@@2W4Enum@@B" = weak_odr dllexport constant i32 2, align 4
// CHECK-NOT: NonExported
