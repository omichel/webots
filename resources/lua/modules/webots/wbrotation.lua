local M = {}

-- assumptions: a rotation is a table with x, y, z and a ids
--             and angle is in radians
--             e.g. r = { x = 0; y = 1; z = 0; a = 1.5708 }
--             a quaternion is a table with w, x, y, and z ids
--             a vector is a table with x, y and z ids
--             a matrix 3x3 is a table with 9 elements

function M.equal(rA, rB)
  return rA.x == rB.x and rA.y == rB.y and rA.z == rB.z and rA.a == rB.a
end

function M.fromquaternion(q)
  r = {}
  r["a"] = 2 * math.acos(q.w)
  if (r.a < 0.0001) then
    r["x"] = 0
    r["y"] = 1
    r["z"] = 0
    r["a"] = 0
  else
    -- normalize axes
    local n = math.sqrt(q.x * q.x + q.y * q.y + q.z * q.z)
    r["x"] = q.x / n
    r["y"] = q.y / n
    r["z"] = q.z / n
  end

  return r
end

function M.frommatrix3(m)
  local r = {}
  local cosAngle = 0.5 * (m[1] + m[5] + m[9] - 1)
  if math.abs(cosAngle) > 1 then
    r["x"] = 1
    r["y"] = 0
    r["z"] = 0
    r["a"] = 0
  else
    r["x"] = m[8] - m[6]
    r["y"] = m[3] - m[7]
    r["z"] = m[4] - m[2]
    r["a"] = math.acos(cosAngle)
  end
  return r
end

function M.toquaternion(r)
  M.normalize(r)
  local halfAngle = r.a * 0.5
  local sinHalfAngle = math.sin(halfAngle)
  local cosHalfAngle = math.cos(halfAngle)
  q = {
    w = cosHalfAngle,
    x = r.x * sinHalfAngle,
    y = r.y * sinHalfAngle,
    z = r.z * sinHalfAngle
  }
  return q
end

function M.tomatrix3(r)
  local c = math.cos(r.a)
  local s = math.sin(r.a)
  local t1 = 1 - c;
  local t2 = r.x * r.z * t1;
  local t3 = r.x * r.y * t1;
  local t4 = r.y * r.z * t1;

  m = {}
  m[1] = r.x * r.x * t1 + c;
  m[2] = t3 - r.z * s;
  m[3] = t2 + r.y * s;
  m[4] = t3 + r.z * s;
  m[5] = r.y * r.y * t1 + c;
  m[6] = t4 - r.x * s;
  m[7] = t2 - r.y * s;
  m[8] = t4 + r.x * s;
  m[9] = r.z * r.z * t1 + c;

  return m
end

function M.isvalid(r)
  return r.x ~= 0 or r.y ~= 0 or r.z ~= 0
end

function M.isidentity(r)
  return r.a == 0.0
end

function M.normalizeangle(r)
  while r.a < -math.pi do
    r.a = r.a + 2 * math.pi
  end
  while r.a > math.pi do
    r.a = r.a - 2 * math.pi
  end
end

function M.normalizeaxis(r)
  if M.isvalid(r) == false then
    r.x = 0
    r.y = 1
    r.z = 0
  end

  local invl = 1 / math.sqrt(r.x * r.x + r.y * r.y + r.z * r.z)
  r.x = r.x * invl
  r.y = r.y * invl
  r.z = r.z * invl
end

function M.normalize(r)
  M.normalizeaxis(r)
  M.normalizeangle(r)
  return r
end

function M.combine(rA, rB)
  qA = M.toquaternion(rA)
  qB = M.toquaternion(rB)

  q = {
    w = qA.w * qB.w - qA.x * qB.x - qA.y * qB.y - qA.z * qB.z,
    x = qA.w * qB.x + qA.x * qB.w + qA.y * qB.z - qA.z * qB.y,
    y = qA.w * qB.y - qA.x * qB.z + qA.y * qB.w + qA.z * qB.x,
    z = qA.w * qB.z + qA.x * qB.y - qA.y * qB.x + qA.z * qB.w
  }
  r = M.fromquaternion(q)
  return r
end

function M.rotatevector3bymatrix3(m, v)
  vector3 = {
    x = m[1] * v.x + m[2] * v.y + m[3] * v.z,
    y = m[4] * v.x + m[5] * v.y + m[6] * v.z,
    z = m[7] * v.x + m[8] * v.y + m[9] * v.z
  }
  return vector3
end

function M.rotatevector3byrotation(r, v)
  matrix3 = M.tomatrix3(r)
  vector3 = M.rotatevector3bymatrix3(matrix3, v)
  return vector3
end

function M.rotatevector3byquaternion(q, v)
  rotation = M.fromquaternion(q)
  vector3 = M.rotatevector3byrotation(rotation, v)
  return vector3
end

return M
