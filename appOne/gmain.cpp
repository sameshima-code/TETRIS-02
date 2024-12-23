#define _EX
#ifdef _EX
#include<libOne.h>
#include <time.h>
struct DATA {
    int INIT=0;
    int PLAY = 1;
    int OVER = 2;
    int state = INIT;
};
struct BLOCK {
    float size = 50;
    int LoopCount = 0;
    int fall = 0;
    int positionx = 5;
    int positiony = 1;
    int shiftx = 0;
    int revo = 0;
    int kaiten = 0;
    int r = 0;
    int hp = 0;
    int RemainPtnx[4] = { 0 };
    int RemainPtny[4] = { 0 };
};
struct STAGE {
    int row = 20;
    int column = 12;
    int stage[20][12] = { 0 };
    const int SBLO = 0;
    const int ZBLO = 1;
    const int JBLO = 2;
    const int LBLO = 3;
    const int TBLO = 4;
    const int OBLO = 5;
    const int IBLO = 6;
    const int WALL = 7;
    const int BACK = 8;
};
struct PATTERNDATA {
    int ptn[7][3][2] = {
        //ブロックのパターン.
         1,0,  0, 1,  -1,+1,
        -1,0,  0, 1,   1, 1,
        -1,0,  1,0,   1, 1,
        -1,0,  1,0,  -1, 1,
        -1,0,  1,0,   0, 1,
        -1,0, -1,1,  0, 1,
         1,0, -1,0,  -2, 0,
    };
    int PtnNo = rand()%7;
    int Ptnx[4] = { 0 };
    int Ptny[4] = { 0 };
};
void init(struct DATA* d,struct STAGE*s, struct BLOCK* b, struct PATTERNDATA* p) {
    for (int y = 0; y < s->row; y++) {
        for (int x = 0; x < s->column-1; x++) {
            //if()
            s->stage[y][0] = s->WALL;
            s->stage[y][s->column - 1] = s->WALL;
            s->stage[y][x] = s->BACK;
            if (y == s->row - 1) {
                s->stage[s->row - 1][x] = s->WALL;
            }
        }
    }
    b->positionx = 5;
    b->positiony = 1;
    b->revo = 0;
    p->PtnNo = rand() % 7;
    d->state = d->PLAY;
}
void decide_color(int i) {
    switch (i) {
    case 0:
        fill(0, 255, 0);//Sミノ 緑.
        break;
    case 1:
        fill(255, 0, 0);//Zミノ 赤.
        break;
    case 2:
        fill(0, 0, 255);//Jミノ 青.
        break;
    case 3:
        fill(255, 128, 0);//Lミノ オレンジ.
        break;
    case 4:
        fill(204, 0, 204);//Tミノ 紫.
        break;
    case 5:
        fill(255, 255, 0);//Oミノ 黄色.
        break;
    case 6:
        fill(51, 255, 255);//Iミノ 水色 .
        break;
    case 7:
        fill(50, 50, 50);//wall.
        break;
    case 8:
        fill(220, 220, 220);//back.
        break;
    }
}
void drawStage(struct STAGE*s,struct BLOCK*b) {
    angleMode(DEGREES);
    for (int y = 0; y < s->row; y++) {
        for (int x = 0; x < s->column; x++) {
            if (s->stage[y][x] >s->IBLO) {
                if (s->stage[y][x] == s->WALL) {
                    decide_color(s->stage[y][x]);
                    rect(b->size * x, b->size * y, b->size, b->size);
                }
                else if (s->stage[y][x] == s->BACK) {
                    decide_color(s->stage[y][x]);
                    rect(b->size * x, b->size * y, b->size, b->size);
                }
            }
            else {
                decide_color(s->stage[y][x]);
                rect(b->size * x, b->size * y, b->size, b->size);
            }
        }
    }
}
void drawBlock(struct STAGE* s, struct BLOCK* b, struct PATTERNDATA* p) {
        decide_color(p->PtnNo);
        rect(b->size * b->positionx, b->size * b->positiony, b->size, b->size);
    for (int i = 0; i < 3; i++) {
        rect(b->size * p->Ptnx[i +1], b->size * p->Ptny[i +1], b->size, b->size);
    }
}
void setPosition(struct BLOCK* b,struct PATTERNDATA*p) {
    p->Ptnx[0] = b->positionx;
    p->Ptny[0] = b->positiony;
    b->kaiten = b->revo % 4;
    for (int i = 0; i < 3; i++) {
        int x = p->ptn[p->PtnNo][i][0];
        int y = p->ptn[p->PtnNo][i][1];
        for (int j = 0; j < b->kaiten; j++) {
            int w = y;
            y = x;
            x = -w;
        }
        p->Ptnx[i + 1] = b->positionx + x;
        p->Ptny[i + 1] = b->positiony + y;
    }
}
void setPtn(struct BLOCK* b,struct STAGE*s, struct PATTERNDATA* p) {
    setPosition(b, p);
    for (int i = 0; i < 4; i++) {
        int x = p->Ptnx[i];
        int y = p->Ptny[i];
        s->stage[y][x] = p->PtnNo;
    }
}
void delPtn(struct BLOCK* b, struct STAGE* s, struct PATTERNDATA* p) {
    setPosition(b, p);
    for (int i = 0; i < 4; i++) {
        int x = p->Ptnx[i];
        int y = p->Ptny[i];
        s->stage[y][x] = s->BACK;
    }
}
int collision(struct BLOCK* b, struct STAGE* s, struct PATTERNDATA* p) {
    setPosition(b, p);
    int flag = 0;
    for (int i = 0; i < 4; i++) {
        int x = p->Ptnx[i];
        int y = p->Ptny[i];
        if (s->stage[y][x] != s->BACK) {
            flag = 1;
        }
    }
    return flag;
}
void memoryBlock(struct BLOCK* b, struct STAGE* s, struct PATTERNDATA* p) {
    int figure[4] = { 0 };
    for (int j = 0; j < 4; j++) {
        figure[j] += p->PtnNo;
    }
    s->stage[b->positiony][b->positionx] = p->PtnNo;
}
void complete(struct STAGE* s) {
    for (int y = 1; y < s->row - 1; y++) {
        int flag = 1;
        for (int x = 1; x < s->column - 1; x++) {
            if (s->stage[y][x] == s->BACK) {
                flag = 0;
            }
        }
        if (flag) {
            for (int upy = y - 1; upy > 0; upy--) {
                for (int x = 1; x < s->column - 1; x++) {
                    s->stage[upy+1][x]=s->stage[upy][x];
                }
            }
        }
    }
}
void operate() {
    textSize(50);
    fill(200);
    text("移動", 600, 50);
    text("←  →", 600, 100);
    text("A   D", 600, 150);
    text("回転", 600, 250);
    text("←  →", 600,300);
    text("J   L", 600, 350);
    text("落下", 600, 450);
    text("S", 640, 500);
    text("リスタート", 600, 600);
    text("  SPACE", 610, 650);
}
void play(struct DATA* d,struct STAGE*s,struct BLOCK*b, struct PATTERNDATA* p) {
    srand((unsigned int)time(0));
    delPtn(b, s, p);
    b->shiftx = 0;
    b->fall = 0;
    b->r = 0;
    if (isPress(KEY_S)) {
        if ((++b->LoopCount %= 8) == 0) {
            b->fall = 1;
        }
    }
    else {
        if ((++b->LoopCount %= 30) == 0) {
            b->fall = 1;
        }
    }
    if (isTrigger(KEY_D))b->shiftx = 1;
    if (isTrigger(KEY_A))b->shiftx = -1;
    //回転.
    if (isTrigger(KEY_L))b->r = 1;
    if (isTrigger(KEY_J))b->r = -1;
    b->positionx += b->shiftx;
        b->positiony += b->fall;
        if (b->r == -1) {
            b->r = 3;
        }
        if (p->PtnNo != s->OBLO) {
            b->revo += b->r;
        }
        //当たり判定.
    if (collision(b,s,p)) {
        b->positiony -= b->fall;
        b->positionx -= b->shiftx;
        b->revo -= b->r;
        if (b->fall > 0 && b->shiftx == 0 && b->r == 0) {
            memoryBlock(b, s, p);
            setPtn(b, s, p);
            complete(s);
            b->positionx = 5;
            b->positiony = 1;
            b->revo = 0;
            p->PtnNo=rand()%7;
            if (collision(b, s, p)) {
                d->state = d->OVER;
            }
        }
    }
    if (isTrigger(KEY_SPACE)) {
        d->state = d->INIT;
    }
    clear(0);
    setPtn(b, s, p);
    drawBlock(s,b,p);
    drawStage(s, b);
    operate();
}
void over(struct DATA* d, struct STAGE* s, struct BLOCK* b, struct PATTERNDATA* p) {
    clear(0);
    drawBlock(s, b, p);
    drawStage(s, b);
    fill(128);
    textSize(50);
    text("Game\nOver", 200, 50);
    fill(200);
    text("Restart", 600, 50);
    text("Press", 600, 100);
    text("'SPACE'", 600, 150);
    if (isTrigger(KEY_SPACE)) {
        d->state = d->INIT;
    }
}
void gmain() {
    struct DATA d;
    struct BLOCK b;
    struct STAGE s;
    struct PATTERNDATA p;
    window(b.size * s.column + 250, b.size * s.row);
    while (notQuit) {
        if (d.state == d.INIT)init(&d,&s,&b,&p);
        else if (d.state == d.PLAY)play(&d,&s,&b,&p);
        else if (d.state == d.OVER)over(&d,&s,&b,&p);
    }
}
#endif



#ifdef _SAMPLE2
#include"libOne.h"
//外部変数------------------------------------------------------------
//シーケンス
const int INIT = 0;
const int PLAY = 1;
const int OVER = 2;
int GameState = INIT;
//色データ
float Satu = 200;
float Value = 255;
float Alpha = 255;
struct COLOR Color[9] = {
    COLOR(0,Satu,Value,Alpha),
    COLOR(30,Satu,Value,Alpha),
    COLOR(60,Satu,Value,Alpha),
    COLOR(120,Satu,Value,Alpha),
    COLOR(180,Satu,Value,Alpha),
    COLOR(220,Satu,Value,Alpha),
    COLOR(300,Satu,Value,Alpha),
    COLOR(200,44,88,Alpha),
    COLOR(0,0,20,Alpha),
};
//壁と背景の色番号
const int WALL = 7;
const int BACK = 8;
//ステージ（色番号を記憶する）
const int ROWS = 20;
const int COLS = 12;
int Stage[ROWS][COLS];
float Size = 50;
//基準ブロック位置と回転数
int X, Y, R;
//ブロックパターンの位置
int Px[4], Py[4];
//ブロック移動制御
int FallFlag = 0;
int LoopCnt = 0;
//ブロックパターン番号  ※ 兼色番号 ※
int PtnNo;
//ブロックパターンオフセット
int PtnOffsets[7][3][2] = {
    //■□■■...0
    -1,0,   1,0,  2,0,
    //■
    //■□■....1
    -1,-1, -1,0,  1,0,
    //  　■
    //■□■....2
    -1,0,   1,-1, 1,0,
    //■□
    //　■■....3
    -1,0,   0,1,  1,1,
    //　□■
    //■■  ....4
     1,0,   0,1, -1,1,
    //　■
    //■□■....5
    -1,0,   0,-1, 1,0,
    //□■
    //■■......6
     1,0,   0,1,  1,1,
};

//関数----------------------------------------------------------------
void setPtnPosition();

void init() {
    //壁と背景の色番号をStage2次元配列にセット
    for (int y = 0; y < ROWS; y++) {
        Stage[y][0] = Stage[y][COLS - 1] = WALL;
        for (int x = 1; x < COLS-1; x++) {
            Stage[y][x] = BACK;
            if (y == ROWS - 1) {
                Stage[y][x] = WALL;
            }
        }
    }
    //ブロック初期値
    X = 5;
    Y = 1;
    R = 0;
    PtnNo = random() % 7;
    setPtnPosition();
    //次のシーケンス
    GameState = PLAY;
}

void drawStage() {
    angleMode(DEGREES);
    colorMode(HSV);
    strokeWeight(20);
    rectMode(CENTER);
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            int no = Stage[y][x];
            stroke(Color[no]);
            fill(0, 0, 100);
            rect(Size/2+Size * x, Size/2+Size * y, Size/2, Size/2);
        }
    }
}
void setPtnPosition() {
    //基準ブロック位置
    Px[0] = X;
    Py[0] = Y;
    //回転数
    int r = R % 4;
    for (int i = 0; i < 3; i++) {
        //現在のブロックパターンのオフセット値を取得
        int ofsX = PtnOffsets[PtnNo][i][0];
        int ofsY = PtnOffsets[PtnNo][i][1];
        //回転数に応じて９０度ずつ回転させる
        for (int j = 0; j < r; j++) {
            int w = ofsX;
            ofsX = -ofsY;
            ofsY = w;
        }
        //位置決定
        Px[i + 1] = X + ofsX;
        Py[i + 1] = Y + ofsY;
    }
}
void setPtnNoToStage() {
    setPtnPosition();
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = PtnNo;
    }
}
void delPtnNoFromStage() {
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = BACK;
    }
}
int collision() {
    setPtnPosition();
    int flag = 0;
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        if (Stage[y][x] != BACK) {
            flag = 1;
        }
    }
    return flag;
}
void complete() {
    for (int y = 1; y < ROWS - 1; y++) {
        int flag = 1;
        for (int x = 1; x < COLS - 1; x++) {
            if (Stage[y][x] == BACK) {
                flag = 0;
            }
        }
        if (flag) {
            //そろったのでスライド
            for (int upy = y - 1; upy >= 0; upy--) {
                for (int x = 1; x < COLS - 1; x++) {
                    Stage[upy + 1][x] = Stage[upy][x];
                }
            }
        }
    }
}
void play() {
    //現在のパターン番号（色番号）をステージから消す
    delPtnNoFromStage();
    //移動、回転
    int dy = 0, dx = 0, dr = 0;;
    if ((++LoopCnt %= 15) == 0)dy = 1;
    if (isTrigger(KEY_D))dx = 1;
    if (isTrigger(KEY_A))dx = -1;
    if (isTrigger(KEY_W))dr = 1;
    if (isTrigger(KEY_S))FallFlag = 1;
    if (FallFlag)dy = 1;
    Y += dy;
    X += dx;
    R += dr;
    if (collision()) {
        //元の位置、回転に戻す
        Y -= dy;
        X -= dx;
        R -= dr;
        FallFlag = 0;
        if (dy == 1 && dx == 0 && dr == 0) {
            //積もらせる
            setPtnNoToStage();
            //行がそろっていたらスライドさせる
            complete();
            //新しいブロック誕生
            X = 5;
            Y = 1;
            R = 0;
            PtnNo = random() % 7;
            if (collision()) {
                GameState = OVER;
            }
        }
    }
    //パターン番号（色番号）をステージにセット
    setPtnNoToStage();

    //描画
    clear(0);
    drawStage();
}

void over() {
    clear(0);
    drawStage();
    //ゲームオーバーテキスト
    fill(0, 0, 255);
    textSize(Size);
    text("ＧａｍｅＯｖｅｒ", (width - Size * 8) / 2, Size);
    if (isTrigger(KEY_SPACE)) {
        GameState = INIT;
    }
}

void gmain() {
    window(Size * COLS, Size * ROWS);
    while (notQuit) {
        if      (GameState == INIT)init();
        else if (GameState == PLAY)play();
        else if (GameState == OVER)over();
    }
}


#endif

#ifdef _SAMPLE1
#include"libOne.h"
//外部変数------------------------------------------------------------
//シーケンス
const int INIT = 0;
const int PLAY = 1;
const int OVER = 2;
int GameState = INIT;
//色データ
float Satu = 200;
float Value = 255;
float Alpha = 255;
struct COLOR Color[9] = {
    COLOR(0,Satu,Value,Alpha),
    COLOR(30,Satu,Value,Alpha),
    COLOR(60,Satu,Value,Alpha),
    COLOR(120,Satu,Value,Alpha),
    COLOR(180,Satu,Value,Alpha),
    COLOR(220,Satu,Value,Alpha),
    COLOR(300,Satu,Value,Alpha),
    COLOR(200,44,88,Alpha),
    COLOR(0,0,20,Alpha),
};
//壁と背景の色番号
const int WALL = 7;
const int BACK = 8;
//ステージ（色番号を記憶する）
const int ROWS = 20;
const int COLS = 12;
int Stage[ROWS][COLS];
float Size = 50;
//ブロックの回転数
int R;
//ブロックパターンの位置（基準は[0]）
int Px[4], Py[4];
//ブロック移動制御
int FallFlag = 0;
int LoopCnt = 0;
//ブロックパターン番号  ※ 兼色番号 ※
int PtnNo;
//ブロックパターンオフセット値(基準[0]からずらす値)
int PtnOffsets[7][3][2] = {
    //■□■■...0
    {{-1,0}, {1,0}, {2,0}},
    //■
    //■□■....1
    {{-1,-1},{-1,0},{1,0}},
    //  　■
    //■□■....2
    {{-1,0}, {1,0}, {1,-1}},
    //■□
    //　■■....3
    {{-1,0}, {0,1}, {1,1}},
    //　□■
    //■■  ....4
    {{ 1,0}, {0,1}, {-1,1}},
     //　■
     //■□■....5
    {{-1,0}, {0,-1}, {1,0}},
     //□■
     //■■......6
    {{1,0},  {0,1},  {1,1}},
};

//関数----------------------------------------------------------------
void init() {
    //壁と背景の色番号をStage 2次元配列 にセット
    for (int y = 0; y < ROWS; y++) {
        Stage[y][0] = Stage[y][COLS - 1] = WALL;
        for (int x = 1; x < COLS - 1; x++) {
            Stage[y][x] = BACK;
            if (y == ROWS - 1) {
                Stage[y][x] = WALL;
            }
        }
    }
    //ブロック初期位置（開始時は４つとも同じでよい）
    for (int i = 0; i < 4; i++) {
        Px[i] = 5;
        Py[i] = 1;
    }
    R = 0;
    PtnNo = random() % 7;
    //次のシーケンス
    GameState = PLAY;
}

void drawStage() {
    angleMode(DEGREES);
    colorMode(HSV);
    strokeWeight(20);
    rectMode(CENTER);
    float px, py, halfSize = Size / 2;
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            //Stage配列の番号に従った色のrectを描画する
            int no = Stage[y][x];
            stroke(Color[no]);
            fill(0, 0, 180);
            px = halfSize + Size * x;
            py = halfSize + Size * y;
            rect(px, py, halfSize, halfSize);
        }
    }
}
void setPtnPosition() {
    //回転数
    int r = R % 4;
    for (int i = 0; i < 3; i++) {
        //現在のブロックパターンのオフセット値を取得
        int ofsX = PtnOffsets[PtnNo][i][0];
        int ofsY = PtnOffsets[PtnNo][i][1];
        //回転数に応じて９０度ずつ回転させる
        for (int j = 0; j < r && PtnNo != 6; j++) {
            int w = ofsX;
            ofsX = -ofsY;
            ofsY = w;
        }
        //位置決定
        Px[i + 1] = Px[0] + ofsX;
        Py[i + 1] = Py[0] + ofsY;
    }
}
void delPtnNoFromStage() {
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = BACK;
    }
}
void setPtnNoToStage() {
    setPtnPosition();
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = PtnNo;
    }
}
int collision() {
    //仮の位置を決める
    setPtnPosition();
    //当たっているかチェック
    int flag = 0;
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        if (Stage[y][x] != BACK) {
            flag = 1;
        }
    }
    return flag;
}
void complete() {
    for (int y = 1; y < ROWS - 1; y++) {
        //y行がそろったかチェック
        int flag = 1;
        for (int x = 1; x < COLS - 1; x++) {
            if (Stage[y][x] == BACK) {
                flag = 0;
            }
        }
        if (flag) {
            //そろったのでスライド
            for (int upy = y - 1; upy >= 0; upy--) {
                for (int x = 1; x < COLS - 1; x++) {
                    Stage[upy + 1][x] = Stage[upy][x];
                }
            }
        }
    }
}
void play() {
    //現在のパターン番号（色番号）をステージから消す
    delPtnNoFromStage();
    //移動、回転
    int dy = 0, dx = 0, dr = 0;;
    if ((++LoopCnt %= 15) == 0)dy = 1;
    if (isTrigger(KEY_D))dx = 1;
    if (isTrigger(KEY_A))dx = -1;
    if (isTrigger(KEY_W))dr = 1;
    if (isTrigger(KEY_S))FallFlag = 1;
    if (FallFlag)dy = 1;
    Px[0] += dx;
    Py[0] += dy;
    R += dr;
    if (collision()) {
        //元の位置、回転に戻す
        Px[0] -= dx;
        Py[0] -= dy;
        R -= dr;
        FallFlag = 0;
        if (dy == 1 && dx == 0 && dr == 0) {
            //積もらせる
            setPtnNoToStage();
            //行がそろっていたらスライドさせる
            complete();
            //新しいブロック誕生
            Px[0] = 5;
            Py[0] = 1;
            R = 0;
            PtnNo = random() % 7;
            if (collision()) {
                GameState = OVER;
            }
        }
    }
    //パターン番号（色番号）をステージにセット
    setPtnNoToStage();

    //描画
    clear(0);
    drawStage();
}

void over() {
    clear(0);
    drawStage();
    //ゲームオーバーテキスト
    for (int x = 1; x < COLS - 1; x++)Stage[0][x] = BACK;
    fill(0, 0, 255);
    textSize(Size);
    text("ＧａｍｅＯｖｅｒ", (width - Size * 8) / 2, Size);
    if (isTrigger(KEY_SPACE)) {
        GameState = INIT;
    }
}

void gmain() {
    window(Size * COLS, Size * ROWS, full);
    ShowCursor(false);
    while (notQuit) {
        if (GameState == INIT)init();
        else if (GameState == PLAY)play();
        else if (GameState == OVER)over();
    }
    ShowCursor(true);
}
#endif
