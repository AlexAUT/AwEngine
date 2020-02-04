
#include "aw/util/math/vector.hpp"
#include "glm/geometric.hpp"

constexpr float epsilion = 0.000001f;
constexpr bool useEpsilonTest = true;

namespace aw {

struct TriTriIntersector
{
  // Source: http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/opttritri.txt
  auto operator()(aw::Vec3 v0, aw::Vec3 v1, aw::Vec3 v2, aw::Vec3 u0, aw::Vec3 u1, aw::Vec3 u2) -> bool
  {
    /* compute plane equation of triangle(V0,V1,V2) */
    auto n1 = glm::cross(v1 - v0, v2 - v0);
    auto d1 = -glm::dot(n1, v0);
    /* plane equation 1: N1.X+d1=0 */

    /* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
    auto du0 = glm::dot(n1, u0) + d1;
    auto du1 = glm::dot(n1, u1) + d1;
    auto du2 = glm::dot(n1, u2) + d1;

    /* coplanarity robustness check */
    if constexpr (useEpsilonTest) {
      if (glm::abs(du0) < epsilion) {
        du0 = 0.0;
      }
      if (glm::abs(du1) < epsilion) {
        du1 = 0.0;
      }
      if (glm::abs(du2) < epsilion) {
        du2 = 0.0;
      }
    }
    auto du0du1 = du0 * du1;
    auto du0du2 = du0 * du2;

    if (du0du1 > 0.0f && du0du2 > 0.0f) { /* same sign on all of them + not equal 0 ? */
      return false;                       /* no intersection occurs */
    }

    /* compute plane of triangle (U0,U1,U2) */
    auto n2 = glm::cross(u1 - u0, u2 - u0);
    auto d2 = -glm::dot(n2, u0);
    /* plane equation 2: N2.X+d2=0 */

    /* put V0,V1,V2 into plane equation 2 */
    auto dv0 = glm::dot(n2, v0) + d2;
    auto dv1 = glm::dot(n2, v1) + d2;
    auto dv2 = glm::dot(n2, v2) + d2;

    if constexpr (useEpsilonTest) {
      if (glm::abs(dv0) < epsilion) {
        dv0 = 0.0;
      }
      if (glm::abs(dv1) < epsilion) {
        dv1 = 0.0;
      }
      if (glm::abs(dv2) < epsilion) {
        dv2 = 0.0;
      }
    }

    auto dv0dv1 = dv0 * dv1;
    auto dv0dv2 = dv0 * dv2;

    if (dv0dv1 > 0.0f && dv0dv2 > 0.0f) { /* same sign on all of them + not equal 0 ? */
      return false;                       /* no intersection occurs */
    }
    return true;

    /* compute direction of intersection line */
    auto D = glm::cross(n1, n2);

    /* compute and index to the largest component of D */
    auto max = glm::abs(D[0]);
    auto bb = glm::abs(D[1]);
    auto cc = glm::abs(D[2]);
    auto index = 0;
    if (bb > max) {
      max = bb, index = 1;
    }
    if (cc > max) {
      max = cc, index = 2;
    }

    /* this is the simplified projection onto L*/
    auto vp0 = v0[index];
    auto vp1 = v1[index];
    auto vp2 = v2[index];

    auto up0 = u0[index];
    auto up1 = u1[index];
    auto up2 = u2[index];

    /* compute interval for triangle 1 */
    float a;
    float b;
    float c;
    float x0;
    float x1;
    if (newIntervals(n1, v0, v1, v2, u0, u1, u2, vp0, vp1, vp2, dv0, dv1, dv2, dv0dv1, dv0dv2, a, b, c, x0, x1)) {
      return true;
    }

    /* compute interval for triangle 2 */
    float d;
    float e;
    float f;
    float y0;
    float y1;
    if (newIntervals(n1, v0, v1, v2, u0, u1, u2, up0, up1, up2, du0, du1, du2, du0du1, du0du2, d, e, f, y0, y1)) {
      return true;
    }

    float xx;
    float yy;
    float xxyy;
    float tmp;
    xx = x0 * x1;
    yy = y0 * y1;
    xxyy = xx * yy;

    tmp = a * xxyy;
    aw::Vec2 isect1;
    isect1[0] = tmp + b * x1 * yy;
    isect1[1] = tmp + c * x0 * yy;

    tmp = d * xxyy;
    aw::Vec2 isect2;
    isect2[0] = tmp + e * xx * y1;
    isect2[1] = tmp + f * xx * y0;

    sort(isect1[0], isect1[1]);
    sort(isect2[0], isect2[1]);

    return !(isect1[1] < isect2[0] || isect2[1] < isect1[0]);
  }

  bool newIntervals(aw::Vec3 N1, aw::Vec3 V0, aw::Vec3 V1, aw::Vec3 V2, aw::Vec3 U0, aw::Vec3 U1, aw::Vec3 U2,
                    float& VV0, float& VV1, float& VV2, float& D0, float& D1, float& D2, float& D0D1, float& D0D2,
                    float& A, float& B, float& C, float& X0, float& X1)
  {
    if (D0D1 > 0.0f) { /* here we know that D0D2<=0.0 */ /* that is D0, D1 are on the same side, D2 on the other or on
                                                            the plane */
      A = VV2;
      B = (VV0 - VV2) * D2;
      C = (VV1 - VV2) * D2;
      X0 = D2 - D0;
      X1 = D2 - D1;
    } else if (D0D2 > 0.0f) { /* here we know that d0d1<=0.0 */
      A = VV1;
      B = (VV0 - VV1) * D1;
      C = (VV2 - VV1) * D1;
      X0 = D1 - D0;
      X1 = D1 - D2;
    } else if (D1 * D2 > 0.0f || D0 != 0.0f) { /* here we know that d0d1<=0.0 or that D0!=0.0 */
      A = VV0;
      B = (VV1 - VV0) * D0;
      C = (VV2 - VV0) * D0;
      X0 = D0 - D1;
      X1 = D0 - D2;
    } else if (D1 != 0.0f) {
      A = VV1;
      B = (VV0 - VV1) * D1;
      C = (VV2 - VV1) * D1;
      X0 = D1 - D0;
      X1 = D1 - D2;
    } else if (D2 != 0.0f) {
      A = VV2;
      B = (VV0 - VV2) * D2;
      C = (VV1 - VV2) * D2;
      X0 = D2 - D0;
      X1 = D2 - D1;
    } else { /* triangles are coplanar */
      return coplanarTriTri(N1, V0, V1, V2, U0, U1, U2);
    }
    return false;
  }
  void sort(float& a, float& b)
  {
    if (a > b) {
      std::swap(a, b);
    }
  }

  auto coplanarTriTri(aw::Vec3 N, aw::Vec3 V0, aw::Vec3 V1, aw::Vec3 V2, aw::Vec3 U0, aw::Vec3 U1, aw::Vec3 U2) -> bool
  {
    /* first project onto an axis-aligned plane, that maximizes the area */
    /* of the triangles, compute indices: i0,i1. */
    aw::Vec3 A = glm::abs(N);
    int i0;
    int i1;
    if (A[0] > A[1]) {
      if (A[0] > A[2]) {
        i0 = 1; /* A[0] is greatest */
        i1 = 2;
      } else {
        i0 = 0; /* A[2] is greatest */
        i1 = 1;
      }
    } else /* A[0]<=A[1] */
    {
      if (A[2] > A[1]) {
        i0 = 0; /* A[2] is greatest */
        i1 = 1;
      } else {
        i0 = 0; /* A[1] is greatest */
        i1 = 2;
      }
    }

    /* test all edges of triangle 1 against the edges of triangle 2 */
    if (edgeAgainstTriEdges(V0, V1, U0, U1, U2, i0, i1)) {
      return true;
    }
    if (edgeAgainstTriEdges(V1, V2, U0, U1, U2, i0, i1)) {
      return true;
    }
    if (edgeAgainstTriEdges(V2, V0, U0, U1, U2, i0, i1)) {
      return true;
    }

    /* finally, test if tri1 is totally contained in tri2 or vice versa */
    if (pointInsideTri(V0, U0, U1, U2, i0, i1)) {
      return true;
    }
    if (pointInsideTri(U0, V0, V1, V2, i0, i1)) {
      return true;
    }

    return false;
  }
  auto edgeAgainstTriEdges(aw::Vec3 V0, aw::Vec3 V1, aw::Vec3 U0, aw::Vec3 U1, aw::Vec3 U2, int i0, int i1) -> bool
  {
    auto Ax = V1[i0] - V0[i0];
    auto Ay = V1[i1] - V0[i1];                      /* test edge U0,U1 against V0,V1 */
    if (edgeEdgeTest(V0, U0, U1, Ax, Ay, i0, i1)) { /* test edge U1,U2 against V0,V1 */
      return true;
    }
    if (edgeEdgeTest(V0, U1, U2, Ax, Ay, i0, i1)) { /* test edge U2,U1 against V0,V1 */
      return true;
    }
    if (edgeEdgeTest(V0, U2, U0, Ax, Ay, i0, i1)) {
      return true;
    }
    return false;
  }

  bool edgeEdgeTest(aw::Vec3 V0, aw::Vec3 U0, aw::Vec3 U1, float Ax, float Ay, int i0, int i1)
  {
    auto Bx = U0[i0] - U1[i0];
    auto By = U0[i1] - U1[i1];
    auto Cx = V0[i0] - U0[i0];
    auto Cy = V0[i1] - U0[i1];
    auto f = Ay * Bx - Ax * By;
    auto d = By * Cx - Bx * Cy;
    if ((f > 0 && d >= 0 && d <= f) || (f < 0 && d <= 0 && d >= f)) {
      auto e = Ax * Cy - Ay * Cx;
      if (f > 0) {
        if (e >= 0 && e <= f) {
          return true;
        }
      } else {
        if (e <= 0 && e >= f) {
          return true;
        }
      }
    }
    return false;
  }
  auto pointInsideTri(aw::Vec3 V0, aw::Vec3 U0, aw::Vec3 U1, aw::Vec3 U2, int i0, int i1) -> bool
  {
    float a;
    float b;
    float c;
    float d0;
    float d1;
    float d2; /* is T1 completly inside T2? */ /* check if V0 is inside tri(U0,U1,U2) */
    a = U1[i1] - U0[i1];
    b = -(U1[i0] - U0[i0]);
    c = -a * U0[i0] - b * U0[i1];
    d0 = a * V0[i0] + b * V0[i1] + c;

    a = U2[i1] - U1[i1];
    b = -(U2[i0] - U1[i0]);
    c = -a * U1[i0] - b * U1[i1];
    d1 = a * V0[i0] + b * V0[i1] + c;

    a = U0[i1] - U2[i1];
    b = -(U0[i0] - U2[i0]);
    c = -a * U2[i0] - b * U2[i1];
    d2 = a * V0[i0] + b * V0[i1] + c;
    if (d0 * d1 > 0.0) {
      if (d0 * d2 > 0.0) {
        return true;
      }
    }
    return false;
  }
};

} // namespace aw
