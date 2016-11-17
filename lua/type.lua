print(type("hello"))	-->string
print(type(10.4))		-->number
print(type(10))			-->number
print(type(print))		-->function
print(type(type))		-->function
print(type(false))		-->boolean
print(type(nil))		-->nil
print(type(type(10)))	-->string

print(type(function () return 1 end))		--anonymous function

print("--------------")

print(type(x))
x=10
print(type(x))
x="hello"
print(type(x))
