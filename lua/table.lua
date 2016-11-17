tab={0,1,2,3,4,5,6}
print(tab[1])

w={x=0, y=1, z="man"}
print(w.x .. " " .. w.y .. " " .. w.z)

ww={tab, w}
print(ww.tab)
print(ww.w)

qq={x=0, y=1, z={m="man", w="woman"}}
print(qq.z.w)


