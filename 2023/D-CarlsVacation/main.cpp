#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

struct Point {
    double x, y;
    Point operator+(const Point& p) const { return Point{x+p.x, y+p.y}; }
    Point operator-(const Point& p) const { return Point{x-p.x, y-p.y}; }
    Point operator*(double c) const { return Point{c*x, c*y}; }
    double len() const { return hypot(x, y); }
};

// Positive if b points counterclockwise of a.
inline double CrossProd(const Point& a, const Point& b) {
    return a.x*b.y - a.y*b.x;
}

bool Intersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2) {
    double cp1 = CrossProd(a2-a1, b1-a1);
    double cp2 = CrossProd(a2-a1, b2-a1);
    if (cp1 < -1e-9 && cp2 < -1e-9) return false;
    if (cp1 >  1e-9 && cp2 >  1e-9) return false;
    cp1 = CrossProd(b2-b1, a1-b1);
    cp2 = CrossProd(b2-b1, a2-b1);
    if (cp1 < -1e-9 && cp2 < -1e-9) return false;
    if (cp1 >  1e-9 && cp2 >  1e-9) return false;
    return true;
}

int main() {
    Point a1, a2, b1, b2;
    double ah, bh;
    cin >> a1.x >> a1.y >> a2.x >> a2.y >> ah >> b1.x >> b1.y >> b2.x >> b2.y >> bh;
    //边长
    double aSideLen = (a2 - a1).len();
    double bSideLen = (b2 - b1).len();
    //金字塔斜棱长度
    double aDiagLen = sqrt(aSideLen * aSideLen / 2 + ah * ah);
    double bDiagLen = sqrt(bSideLen * bSideLen / 2 + bh * bh);
    //顶点到底边中点长度。勾股定理算
    double aAltLen = sqrt(aSideLen * aSideLen / 4 + ah * ah);
    double bAltLen = sqrt(bSideLen * bSideLen / 4 + bh * bh);
    double ret = 1e9;
    for (int ai = 0; ai < 4; ai++) {
        Point ap{a1.y - a2.y, a2.x - a1.x};
        Point amid{(a1 + a2) * 0.5 + ap * 0.5};
        for (int bi = 0; bi < 4; bi++) {
            Point bp{b1.y - b2.y, b2.x - b1.x};
//金字塔中心投影
            Point bmid{(b1 + b2) * 0.5 + bp * 0.5};
//            ab四条边两两组合遍历
            for (int aDiag = 0; aDiag < 2; aDiag++) {
//                将金字塔顶点旋转到平面上
                Point at = aDiag ? a1 : (a1 + a2) * 0.5 + ap * (aAltLen / aSideLen);
                double alen = aDiag ? aDiagLen : 0.0;
                for (int bDiag = 0; bDiag < 2; bDiag++) {
//                    bDiag=1,从塔尖沿着斜棱走到顶点 ；=0从塔尖走到边上
                    Point bt = bDiag ? b1 : (b1 + b2) * 0.5 + bp * (bAltLen / bSideLen);
                    double blen = bDiag ? bDiagLen : 0.0;
// 针对从a的塔尖走到a1a2上，然后走到b。
// 第一个条件判断的是，如果走到a1a2的方向，不是向b塔靠近，就是白费力气即b塔应该在a1a2的右方，通过bmid判断
//第二个条件判断路径应与a1a2相交
                    if (!aDiag && (CrossProd(bmid - a1, a2 - a1) < 0 || !Intersect(a1, a2, at, bt))) continue;
                    if (!bDiag && (CrossProd(amid - b1, b2 - b1) < 0 || !Intersect(b1, b2, at, bt))) continue;
                    ret = min(ret, alen + blen + (bt - at).len());
                }
            }
//移动到下一个顶点对
            b1 = b2 + bp;
            swap(b1, b2);
        }
        a1 = a2 + ap;
        swap(a1, a2);
    }
    cout << fixed << setprecision(9) << ret << endl;
}