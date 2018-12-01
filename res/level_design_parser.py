import sys

if len(sys.argv) < 3:
    print("File name and level_design path must be passed as arguments")
    sys.exit()

file_name = sys.argv[1]
level_design_path = sys.argv[2]

f = open(level_design_path, "r")

header_file = open("{}.h".format(file_name), "w")

level_tempo = f.readline()
level_len, background_num = f.readline().split()

header_file.write("#ifndef {}_H\n".format(file_name.upper()))
header_file.write("#define {}_H\n\n".format(file_name.upper()))
header_file.write("#define {}_tempo {}".format(file_name, level_tempo))
header_file.write("#define {}_len {}\n\n".format(file_name, level_len))

header_file.write("#include <stdbool.h>\n\n")

header_file.write("extern const short {}_platform_heights[{}];\n".format(file_name, str(level_len)))
header_file.write("extern const bool {}_enemy_present[{}];\n".format(file_name, str(level_len)))
header_file.write("extern const short {}_enemy_type[{}];\n".format(file_name, str(level_len)))
header_file.write("extern const short {}_enemy_heights[{}];\n".format(file_name, str(level_len)))
header_file.write("extern const bool {}_collectable_present[{}];\n".format(file_name, str(level_len)))
header_file.write("extern const short {}_collectable_heights[{}];\n".format(file_name, str(level_len)))

header_file.write("\n#endif")

heights = []
ep = []
et = []
eh = []
cp = []
ch = []

for line in f.readlines():
    args = line.split()

    platform_height = 0
    enemy_present = False
    enemy_type = 0
    enemy_height = 0
    collectable_present = False
    collectable_height = 0

    if len(args) == 0:
        continue

    platform_height = args[0]
    enemy_present = int(args[1])

    if enemy_present:
        enemy_type = args[2]
        enemy_height = args[3]
    else:
        collectable_present = int(args[2])
        if collectable_present:
            collectable_height = args[3]


    heights.append(str(platform_height))
    ep.append(str("1" if enemy_present else "0"))
    et.append(str(enemy_type))
    eh.append(str(enemy_height))
    cp.append(str("1" if collectable_present else "0"))
    ch.append(str(collectable_height))

header_file.close()

cpp_file = open("{}.c".format(file_name), "w") 

cpp_file.write("#include\"{}.h\"\n\n".format(file_name))
cpp_file.write("const short {}_platform_heights[{}] = {{ {} }};\n".format(file_name, str(len(heights)), ', '.join(heights)))
cpp_file.write("const bool {}_enemy_present[{}] = {{ {} }};\n".format(file_name, str(len(ep)), ', '.join(ep)))
cpp_file.write("const short {}_enemy_type[{}] = {{ {} }};\n".format(file_name, str(len(et)), ', '.join(et)))
cpp_file.write("const short {}_enemy_heights[{}] = {{ {} }};\n".format(file_name, str(len(eh)), ', '.join(eh)))
cpp_file.write("const bool {}_collectable_present[{}] = {{ {} }};\n".format(file_name, str(len(cp)), ', '.join(cp)))
cpp_file.write("const short {}_collectable_heights[{}] = {{ {} }};\n".format(file_name, str(len(ch)), ', '.join(ch)))

cpp_file.close()
