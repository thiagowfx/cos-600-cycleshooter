buffer = Procedural.TextureBuffer(128)
Procedural.Image(buffer):setFile("red_brick.jpg"):process()
Procedural.OilPaint(buffer):setRadius(5):process()
tests:addTextureBuffer(buffer)
dotfile = tests:getDotFile("texture_22", "OilPaint_Demo")
dotfile:set("Image", "texture_image", "OilPaint", "texture_oilpaint")
