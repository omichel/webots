local M = {}

-- create a list of 'div' circle coordinates according
-- to a circle centered at {'cx', 'cy'} and rotated by
-- 'shift' radians
function M.circle(radius, div, cx, cy, shift)
  local circle = {}
  local quantum = 2 * math.pi / div
  for i = 0, div do
    table.insert(circle, {x = radius * math.cos(i * quantum + shift) + cx; y = radius * math.sin(i * quantum + shift) + cy})
  end
  return circle
end

-- determine if a point is inside a given polygon or not
-- polygon is a list of (x,y) pairs.
function M.ispointinpolygon(x, y, polygon)
  local wbcore = require('wbcore')
  local n = wbcore.tablelength(polygon)
  if n < 3 then
    return false
  end
  local inside = false
  local p1x = polygon[1].x
  local p1y = polygon[1].y
  local xinters = 0
  for i = 1, n do
    local p2x = polygon[i % n + 1].x
    local p2y = polygon[i % n + 1].y
    if y > math.min(p1y,p2y) then
      if y <= math.max(p1y,p2y) then
        if x <= math.max(p1x,p2x) then
          if p1y ~= p2y then
            xinters = (y-p1y)*(p2x-p1x)/(p2y-p1y)+p1x
          end
          if p1x == p2x or x <= xinters then
            inside = not inside
          end
        end
      end
    end
    p1x = p2x
    p1y = p2y
  end
  return inside
end

-- return the closest points in the array
-- referencepoint should be a 2d point (x, y)
-- pointsarray should be a array of 2d points (x, y)
function M.findclosest2Dpointinarray(referencePoint, pointsArray)
  local point = referencePoint
  local wbcore = require('wbcore')
  local wbvector2 = require('wbvector2')
  local dist = 1 / 0
  local pointsNumber = wbcore.tablelength(pointsArray)
  for i = 1, pointsNumber do
    local currentDistance = wbvector2.distance(referencePoint, pointsArray[i])
    if currentDistance < dist then
      dist = currentDistance
      point = pointsArray[i]
    end
  end
  return point
end

-- check if an array of points (x, y) is defined in a clockwise order
-- based on the formula (x2-x1)(y2+y1) (http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order)
function M.islistclockwise2D(points)
  local wbcore = require('wbcore')
  local pointsNumber = wbcore.tablelength(points)
  local total = 0
  for i = 1, pointsNumber - 1 do
    total = total + (points[i+1].x - points[i].x) * (points[i+1].y + points[i].y)
  end
  total = total + (points[1].x - points[pointsNumber].x) * (points[1].y + points[pointsNumber].y)

  if total >= 0 then
    return false
  else
    return true
  end
end

-- create a B-Spline curve of third order using the array of points (x and y)
-- return then a new array of point following this B-Spline subdividing
-- each segment by 'subdivision'
function M.bspline2D(points, subdivision)
  local spline = {}
  local wbcore = require('wbcore')
  local wbvector2 = require('wbvector2')
  local pointsNumber = wbcore.tablelength(points)

  -- extend the points array
  points[0]                = wbvector2.add(points[1], wbvector2.minus(points[1], points[2]))
  points[-1]               = wbvector2.add(points[0], wbvector2.minus(points[0], points[1]))
  points[pointsNumber + 1] = wbvector2.add(points[pointsNumber], wbvector2.minus(points[pointsNumber], points[pointsNumber - 1]))
  points[pointsNumber + 2] = wbvector2.add(points[pointsNumber + 1], wbvector2.minus(points[pointsNumber + 1], points[pointsNumber]))

  -- Interpolation
  local index = 1
  spline[index] = points[1] -- first point
  for i = 0, pointsNumber - 1 do
    local a = {} -- compute the third order coefficients for x
    local b = {} -- compute the third order coefficients for y
    a[1] = (-points[i-1].x + 3 * points[i].x - 3 * points[i+1].x + points[i+2].x) / 6.0;
    a[2] = (3 * points[i-1].x - 6 * points[i].x + 3 * points[i+1].x) / 6.0;
    a[3] = (-3 * points[i-1].x + 3 * points[i+1].x) / 6.0;
    a[4] = (points[i-1].x + 4 * points[i].x + points[i+1].x) / 6.0;
    b[1] = (-points[i-1].y + 3 * points[i].y - 3 * points[i+1].y + points[i+2].y) / 6.0;
    b[2] = (3 * points[i-1].y - 6 * points[i].y + 3 * points[i+1].y) / 6.0;
    b[3] = (-3 * points[i-1].y + 3 * points[i+1].y) / 6.0;
    b[4] = (points[i-1].y + 4 * points[i].y + points[i+1].y) / 6.0;
    for j = 1, subdivision do
      index = index + 1
      spline[index] = {}
      local t = j / subdivision
      spline[index].x = (a[3] + t * (a[2] + t * a[1])) * t + a[4]
      spline[index].y = (b[3] + t * (b[2] + t * b[1])) * t + b[4]
    end
  end
  return spline
end

-- create a B-Spline curve of third order using the array of points (x, y and z)
-- return then a new array of point following this B-Spline subdividing
-- each segment by 'subdivision'
function M.bspline3D(points, subdivision)
  local spline = {}
  local wbcore = require('wbcore')
  local wbvector3 = require('wbvector3')
  local pointsNumber = wbcore.tablelength(points)

  -- extend the points array
  points[0]                = wbvector3.add(points[1], wbvector3.minus(points[1], points[2]))
  points[-1]               = wbvector3.add(points[0], wbvector3.minus(points[0], points[1]))
  points[pointsNumber + 1] = wbvector3.add(points[pointsNumber], wbvector3.minus(points[pointsNumber], points[pointsNumber - 1]))
  points[pointsNumber + 2] = wbvector3.add(points[pointsNumber + 1], wbvector3.minus(points[pointsNumber + 1], points[pointsNumber]))

  -- Interpolation
  local index = 1
  spline[index] = points[1] -- first point
  for i = 1, pointsNumber - 1 do
    local a = {} -- compute the third order coefficients for x
    local b = {} -- compute the third order coefficients for y
    local c = {} -- compute the third order coefficients for z
    a[1] = (-points[i-1].x + 3 * points[i].x - 3 * points[i+1].x + points[i+2].x) / 6.0;
    a[2] = (3 * points[i-1].x - 6 * points[i].x + 3 * points[i+1].x) / 6.0;
    a[3] = (-3 * points[i-1].x + 3 * points[i+1].x) / 6.0;
    a[4] = (points[i-1].x + 4 * points[i].x + points[i+1].x) / 6.0;
    b[1] = (-points[i-1].y + 3 * points[i].y - 3 * points[i+1].y + points[i+2].y) / 6.0;
    b[2] = (3 * points[i-1].y - 6 * points[i].y + 3 * points[i+1].y) / 6.0;
    b[3] = (-3 * points[i-1].y + 3 * points[i+1].y) / 6.0;
    b[4] = (points[i-1].y + 4 * points[i].y + points[i+1].y) / 6.0;
    c[1] = (-points[i-1].z + 3 * points[i].z - 3 * points[i+1].z + points[i+2].z) / 6.0;
    c[2] = (3 * points[i-1].z - 6 * points[i].z + 3 * points[i+1].z) / 6.0;
    c[3] = (-3 * points[i-1].z + 3 * points[i+1].z) / 6.0;
    c[4] = (points[i-1].z + 4 * points[i].z + points[i+1].z) / 6.0;
    for j = 1, subdivision do
      index = index + 1
      spline[index] = {}
      local t = j / subdivision
      spline[index].x = (a[3] + t * (a[2] + t * a[1])) * t + a[4]
      spline[index].y = (b[3] + t * (b[2] + t * b[1])) * t + b[4]
      spline[index].z = (c[3] + t * (c[2] + t * c[1])) * t + c[4]
    end
  end
  return spline
end

return M
