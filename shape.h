//
// Created by morimori on 2021/04/21.
//

namespace shape {
    struct vec3 {
        vec3(double x_, double y_, double z_) {
            x = x_;
            y = y_;
            z = z_;
        }


        double x;
        double y;
        double z;
    };

    enum axis {
        X, Y, Z
    };

    struct rotation {
        rotation(double angle_, axis axis_, vec3 origin_) {
            angle = angle_;
            axis = axis_;
            origin = origin_;
        }

        double angle;
        axis axis;
        vec3 origin = vec3(0, 0, 0);
    };

    struct box {
        box(vec3 start_, vec3 end_) {
            start = start_;
            end = end_;
        }

        box(double stx, double sty, double stz, double enx, double eny, double enz) {
            start = vec3(stx, sty, stz);
            end = vec3(enx, eny, enz);
        }

        vec3 start = vec3(0, 0, 0);
        vec3 end = vec3(0, 0, 0);
    };

    struct anglebox : public box {
        anglebox(vec3 start_, vec3 end_, rotation arotation_) : box(start_, end_) {
            arotation = arotation_;
        }

        rotation arotation = rotation(0, X, vec3(0, 0, 0));
    };
}