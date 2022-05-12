class Worker {
private:
    bool takeA;
    bool takeB;
    bool leftPick;
    bool rightPick;
    static int numberProduct;
public:

    Worker ();

    ~Worker ();

    bool getTakeA ();

    bool getTakeB ();

    bool getLeftPick ();

    bool getRightPick ();

    void setTakeA(bool takeA);

    void setTakeB(bool takeB);

    void setLeftPick (bool leftPick);

    void setRightPick (bool rightPick);

    void leftArmPickA ();

    void leftArmPickB ();

    void rightArmPickA ();

    void rightArmPickB ();

    void assembleProduct ();

    void putProduct ();

    static void increaseNumberProduct ();

    static void setNumberProduct(int n);

    static int getNumberProduct();

};