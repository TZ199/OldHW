def parse_line(txt):
    txt = txt.split("/",3)
    if txt[0].isdigit()== False or txt[1].isdigit()== False or txt[2].isdigit()== False:
        return None
    return (int(txt[0]), int(txt[1]),int(txt[2]),txt[3])

print parse_line("12/3/4/Here is some random text, like 5/4=3")
print parse_line("as12/3/4/Here is some random text, like 5/4=3")
print parse_line("12/a/412/3/4/Here is some random text, like 5/4=3")
print parse_line("12/32/4 Here is some spaces ")
print parse_line("12/12/12/   Again some spaces\n")