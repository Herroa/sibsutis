class Sally {
   copy() : SELF_TYPE { self };
};

class F { 
    --s : String <- "hello";
    (*g (y:String) : String {
       (*y.concat(s)*)
    };*)
    f (x:Int) : Int {
       x/1
    };
}; 

class Main {
   x : Sally <- (new Sally).copy();

   main() : Sally { x };
};