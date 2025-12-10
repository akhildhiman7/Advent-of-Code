# ✅ Editorial — Day 9: Movie Theater (Part 2)

We are given an **ordered cyclic list of red tiles**. Between each consecutive red tile there is a straight horizontal or vertical chain of green tiles. Together, these red + green tiles form a **single closed, axis-aligned polygon with no holes**. All tiles strictly inside this loop are also green.

Our task is to find the **largest axis-aligned rectangle** such that:

1. **Opposite corners are red tiles**, and
2. **Every tile on the rectangle boundary lies inside or on the polygon** (i.e., is either red or green).

Because the polygon has **no holes**, if the entire boundary of a rectangle lies inside/on the polygon, then its entire interior is guaranteed to be green.

---

## 🔹 High-Level Strategy

1. Treat the given red points as the **vertices of an orthogonal polygon**.
2. Enumerate **all possible rectangles** formed by choosing **two red points as diagonal corners**.
3. For each rectangle:

   * Check that **all four corners lie inside or on the polygon**.
   * Then verify that **every part of the rectangle boundary stays inside the polygon**.
4. Track the **maximum valid rectangle area**.

No grid, no flood fill, no forbidden-tile brute force.

---

## 🔹 Key Geometric Insight

Because the polygon is:

* **Axis-aligned**
* **Simple (no self-intersections)**
* **Hole-free**

we can validate a rectangle using only:

* **Point-in-polygon tests**
* And **scanline sampling of the rectangle boundary**

We do *not* need to test every tile along the boundary individually.

---

# ✅ Step-by-Step Algorithm

---

## ✅ Step 1: Build the Polygon

Input:

```
points = [(x₀,y₀), (x₁,y₁), ..., (xₙ₋₁,yₙ₋₁)]
```

These points are already given in **loop order**, so:

* Each adjacent pair forms a polygon edge.
* The last point connects back to the first.

No sorting or reconstruction is needed.

---

## ✅ Step 2: Precompute Scan Levels (Your Reuse Optimization)

To avoid re-running full polygon checks for every single boundary coordinate, we precompute **stable sampling lines**:

1. Extract all **unique X-coordinates** from the polygon → `uniqX`
2. Extract all **unique Y-coordinates** from the polygon → `uniqY`
3. Sort both lists.
4. Build:

   * `midX[i] = (uniqX[i] + uniqX[i+1]) / 2`
   * `midY[i] = (uniqY[i] + uniqY[i+1]) / 2`

These midpoints represent **regions where inside/outside does not change unless a polygon edge is crossed**.

This is the crucial part that implements your:

> “Check y₂ first, store the result, then reuse it moving downwards.”

Instead of stepping one unit at a time, we jump **only at levels where geometry can change**.

---

## ✅ Step 3: Enumerate Rectangle Candidates

For every pair of red points `Pi , Pj`:

* If `xi == xj` or `yi == yj`, they **cannot form a diagonal** → skip.
* Otherwise:

  ```
  lx = min(xi, xj)
  rx = max(xi, xj)
  ly = min(yi, yj)
  ry = max(yi, yj)
  ```

This defines one rectangle.

Total candidates:

```
O(R²)
```

where `R = number of red points`.

---

## ✅ Step 4: Boundary Validation (The Core of Your Solution)

A rectangle is valid **if and only if its entire boundary lies inside or on the polygon**.

We verify this in **three stages**:

---

### ✅ (A) Corner Check

Check these four corners using a **boundary-aware point-in-polygon test**:

* `(lx, ly)`
* `(rx, ly)`
* `(rx, ry)`
* `(lx, ry)`

Rules for the test:

* If a point lies **exactly on any polygon edge**, it is treated as **inside**.
* The ray-casting parity logic uses a **half-open rule** to avoid double-counting vertices.
* A continuous collinear boundary is treated as **one hit**, not multiple.

If **any corner is outside**, the rectangle is invalid.

---

### ✅ (B) Vertical Edge Scan (Left & Right Sides)

For every `midY`:

* If `ly < midY < ry`:

  * Check if `(lx, midY)` is inside/on the polygon.
  * Check if `(rx, midY)` is inside/on the polygon.

If either fails → the rectangle leaks outside → **reject**.

This ensures **every vertical part of the rectangle boundary** is valid.

---

### ✅ (C) Horizontal Edge Scan (Top & Bottom Sides)

For every `midX`:

* If `lx < midX < rx`:

  * Check if `(midX, ly)` is inside/on the polygon.
  * Check if `(midX, ry)` is inside/on the polygon.

If either fails → **reject**.

This ensures **every horizontal part of the rectangle boundary** is valid.

---

### ✅ If All Three Tests Pass

Then **the entire rectangle boundary lies inside or on the polygon**, so the rectangle is valid.

---

## ✅ Step 5: Compute Area

For each valid rectangle:

```
area = (rx - lx + 1) × (ry - ly + 1)
```

Keep the maximum.

---

# ✅ Final Time Complexity

Let:

* `R = number of red points`

Then:

| Phase                         | Complexity    |
| ----------------------------- | ------------- |
| Preprocessing unique X/Y      | `O(R log R)`  |
| Rectangle enumeration         | `O(R²)`       |
| Boundary checks per rectangle | `O(R)`        |
| **Total**                     | ✅ **`O(R³)`** |

With:

```
R = 496  →  ~1.2 × 10⁸ operations
```

This comfortably runs in C++.

---

# ✅ Why This Algorithm Is Correct

* ✅ Handles **spikes and thin corridors**.
* ✅ Correctly handles:

  * Collinear overlaps
  * Vertex touches
  * Continuous polygon boundaries
* ✅ Avoids:

  * Flood fill
  * Huge grids
  * Forbidden-tile constructions
* ✅ Directly implements:

  > “The rectangle boundary must remain within the polygon.”

---

# ✅ One-Sentence Summary (Editorial Style)

> We enumerate all rectangle candidates defined by pairs of red vertices and validate each by verifying that all four corners and all vertical and horizontal boundary segments lie inside or on the orthogonal polygon using boundary-aware point-in-polygon tests sampled at precomputed scanline midpoints.
