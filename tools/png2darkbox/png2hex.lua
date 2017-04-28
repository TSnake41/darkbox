require 'pngLua.png'

if #arg < 1 then
    print "Usage : lua png2hex.lua input.png > output.hex"
    os.exit(0)
end

local image = pngImage(arg[1], nil)

if image == nil then
    error "ERROR: Could not read image."
end

for y=1,image.height,1 do
    for x=1,image.width,1 do
        local pixel = image:getPixel(x,y)
        io.write(string.format("%02X%02X%02X ", pixel.R, pixel.G, pixel.B))
    end
    io.write '\n'
end
