s3 = Procedural.CircleShape():setNumSeg(16):realizeShape()
ms = Procedural.MultiShape():addShape(Procedural.Shape(s3):scale(1.1)):addShape(s3:switchSide())
p3 = Procedural.CatmullRomSpline3():addPoint(0,0,-5):addPoint(0,0,0):addPoint(1,-1,5):realizePath()	
tb = Procedural.Extruder():setMultiShapeToExtrude(ms):setExtrusionPath(p3):buildTriangleBuffer()
tests:addTriangleBuffer(tb)