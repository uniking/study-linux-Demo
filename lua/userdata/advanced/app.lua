ma = require "myArray"  --指定包名称

p = ma.new(256)
print(type(p))
ma.set(p, 1, 10)
print(ma.get(p, 1))
print(ma.size(p))
