#include "bits/stdc++.h"

using namespace std;

struct Point{
    double x,y;
    Point operator+(const Point& p) const { return Point{x+p.x, y+p.y}; }
    Point operator-(const Point& p) const { return Point{x-p.x, y-p.y}; }
    Point operator*(double c) const { return Point{c*x, c*y}; }
    Point operator/(double c) const { return Point{x/c, y/c}; }
    double len() const { return hypot(x, y); }
};

double crossProd(const Point& a, const Point& b){
    return a.x * b.y - a.y * b.x;
}

bool intersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2) {
    auto cp1 = crossProd(a2-a1, b1-a1);
    auto cp2 = crossProd(a2-a1, b2-a1);
    if(cp1<0 && cp2<0 || cp1>0 && cp2>0)
        return false;
    auto cp3 = crossProd(b2-b1, a1-b1);
    auto cp4 = crossProd(b2-b1, a2-b1);
    if(cp3<0 && cp4<0 || cp3>0 && cp4>0)
        return false;
    return true;
}

int main(){
    Point a1,a2,b1,b2;
    double ah,bh;
    cin>>a1.x>>a1.y>>a2.x>>a2.y>>ah;
    cin>>b1.x>>b1.y>>b2.x>>b2.y>>bh;
    double alen=(a2-a1).len();
    double blen=(b2-b1).len();
    double aDiagLen=sqrt(alen*alen/2+ah*ah);
    double bDiagLen=sqrt(blen*blen/2+bh*bh);
    double aAltLen=sqrt(alen*alen/4+ah*ah);
    double bAltLen=sqrt(blen*blen/4+bh*bh);
    double ret=1e9;
    for(int ai=0;ai<4;++ai){
        Point ap{a1.y-a2.y, a2.x-a1.x};
        for(int bi=0;bi<4;++bi){
            Point bp{b1.y-b2.y, b2.x-b1.x};
            for(int aDiag=0;aDiag<=1;++aDiag){
                Point at = aDiag? a1 : (a1+a2)*0.5 + ap/alen*aAltLen;
                double alen=aDiag? aDiagLen : 0.0;
                for(int bDiag=0;bDiag<=1;++bDiag){
                    Point bt = bDiag? b1 : (b1+b2)*0.5 + bp/blen*bAltLen;
                    double blen=bDiag? bDiagLen : 0.0;
                    if(!aDiag && (crossProd(a2-a1,bt-a1)>0 || !intersect(a1,a2,at,bt)))
                        continue;
                    if(!bDiag && (crossProd(b2-b1,at-b1)>0 ||!intersect(b1,b2,at,bt)))
                        continue;
                    ret=min(ret, (bt-at).len()+alen+blen);
                }
            }
            b1=b2+bp; swap(b1,b2);
        }
        a1=a2+ap; swap(a1,a2);
    }
    cout<<fixed<<setprecision(9)<<ret<<endl;
}