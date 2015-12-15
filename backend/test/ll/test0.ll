
define i8* @"int -> list_int -> list_int"(i8*) {
  %2 = call i8* @malloc(i64 16)
  %3 = bitcast i8* %2 to { i8*, i8* }*
  %4 = getelementptr { i8*, i8* }, { i8*, i8* }* %3, i32 0, i32 1
  %5 = getelementptr inbounds i8, i8* %0, i64 -8
  %6 = bitcast i8* %5 to i8**
  %7 = load i8*, i8** %6
  store i8* %7, i8** %4
  %8 = getelementptr { i8*, i8* }, { i8*, i8* }* %3, i32 0, i32 0
  %9 = getelementptr inbounds i8, i8* %5, i64 -8
  %10 = bitcast i8* %9 to i8**
  %11 = load i8*, i8** %10
  store i8* %11, i8** %8
  %12 = bitcast { i8*, i8* }* %3 to i8*
  ret i8* %12
}

declare i8* @malloc(i64)

define i8* @"int -> list_int -> list_int1"(i8*) {
  %2 = call i8* @malloc(i64 16)
  %3 = bitcast i8* %2 to { i8* (i8*)*, i8* }*
  %4 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %3, i32 0, i32 0
  %5 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %3, i32 0, i32 1
  store i8* (i8*)* @"int -> list_int -> list_int", i8* (i8*)** %4
  store i8* %0, i8** %5
  %6 = bitcast { i8* (i8*)*, i8* }* %3 to i8*
  ret i8* %6
}

define i8* @0(i8*) {
  %2 = call i8* @malloc(i64 16)
  %3 = call i8* @malloc(i64 16)
  %4 = bitcast i8* %3 to { i8* (i8*)*, i8* }*
  %5 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %4, i32 0, i32 0
  %6 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %4, i32 0, i32 1
  store i8* (i8*)* @"int -> list_int -> list_int1", i8* (i8*)** %5
  store i8* %2, i8** %6
  %7 = bitcast { i8* (i8*)*, i8* }* %4 to i8*
  ret i8* %7
}

define i8* @"unit -> list_int"(i8*) {
  %2 = call i8* @malloc(i64 16)
  %3 = bitcast i8* %2 to { i32, i8* }*
  %4 = getelementptr inbounds i8, i8* %0, i64 -8
  %5 = bitcast i8* %4 to i8**
  %6 = load i8*, i8** %5
  %7 = getelementptr { i32, i8* }, { i32, i8* }* %3, i32 0, i32 0
  %8 = getelementptr { i32, i8* }, { i32, i8* }* %3, i32 0, i32 1
  store i32 0, i32* %7
  store i8* %6, i8** %8
  %9 = bitcast { i32, i8* }* %3 to i8*
  ret i8* %9
}

define i8* @1(i8*) {
  %2 = call i8* @malloc(i64 8)
  %3 = bitcast i8* %2 to i8**
  %4 = bitcast i8** %3 to i8*
  %5 = call i8* @malloc(i64 16)
  %6 = bitcast i8* %5 to { i8* (i8*)*, i8* }*
  %7 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %6, i32 0, i32 0
  %8 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %6, i32 0, i32 1
  store i8* (i8*)* @"unit -> list_int", i8* (i8*)** %7
  store i8* %4, i8** %8
  %9 = bitcast { i8* (i8*)*, i8* }* %6 to i8*
  ret i8* %9
}

define i8* @"list_int_y -> list_int"(i8*) {
  %2 = call i8* @malloc(i64 16)
  %3 = bitcast i8* %2 to { i32, i8* }*
  %4 = getelementptr inbounds i8, i8* %0, i64 -8
  %5 = bitcast i8* %4 to i8**
  %6 = load i8*, i8** %5
  %7 = getelementptr { i32, i8* }, { i32, i8* }* %3, i32 0, i32 0
  %8 = getelementptr { i32, i8* }, { i32, i8* }* %3, i32 0, i32 1
  store i32 1, i32* %7
  store i8* %6, i8** %8
  %9 = bitcast { i32, i8* }* %3 to i8*
  ret i8* %9
}

define i8* @2(i8*) {
  %2 = call i8* @malloc(i64 8)
  %3 = bitcast i8* %2 to i8**
  %4 = bitcast i8** %3 to i8*
  %5 = call i8* @malloc(i64 16)
  %6 = bitcast i8* %5 to { i8* (i8*)*, i8* }*
  %7 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %6, i32 0, i32 0
  %8 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %6, i32 0, i32 1
  store i8* (i8*)* @"list_int_y -> list_int", i8* (i8*)** %7
  store i8* %4, i8** %8
  %9 = bitcast { i8* (i8*)*, i8* }* %6 to i8*
  ret i8* %9
}

define i8* @l(i8*) {
  %2 = getelementptr inbounds i8, i8* %0, i64 -8
  %3 = bitcast i8* %2 to i8**
  %4 = load i8*, i8** %3
  ret i8* %4
}

define i8* @"abs l"(i8*) {
  %2 = call i8* @malloc(i64 16)
  %3 = bitcast i8* %2 to { i8* (i8*)*, i8* }*
  %4 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %3, i32 0, i32 0
  store i8* (i8*)* @l, i8* (i8*)** %4
  %5 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %3, i32 0, i32 1
  store i8* %0, i8** %5
  %6 = bitcast { i8* (i8*)*, i8* }* %3 to i8*
  ret i8* %6
}

define i8* @umain(i8*) {
  %2 = call i8* @malloc(i64 4096)
  %3 = call i8* @0(i8* %2)
  %4 = bitcast i8* %2 to i8**
  store i8* %3, i8** %4
  %5 = getelementptr inbounds i8, i8* %2, i64 8
  %6 = call i8* @1(i8* %5)
  %7 = bitcast i8* %5 to i8**
  store i8* %6, i8** %7
  %8 = getelementptr inbounds i8, i8* %5, i64 8
  %9 = call i8* @2(i8* %8)
  %10 = bitcast i8* %8 to i8**
  store i8* %9, i8** %10
  %11 = getelementptr inbounds i8, i8* %8, i64 8
  %12 = call i8* @"abs l"(i8* %11)
  %13 = bitcast i8* %12 to { i8* (i8*)*, i8* }*
  %14 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %13, i32 0, i32 0
  %15 = getelementptr { i8* (i8*)*, i8* }, { i8* (i8*)*, i8* }* %13, i32 0, i32 1
  %16 = load i8* (i8*)*, i8* (i8*)** %14
  %17 = load i8*, i8** %15
  %18 = bitcast i8* %17 to i8**
  store i8* %0, i8** %18
  %19 = getelementptr inbounds i8, i8* %17, i64 8
  %20 = call i8* %16(i8* %19)
  ret i8* %20
}
