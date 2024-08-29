import json
with open("input1.txt", "w") as f:
    with open("output3.txt", "r") as file:
        data = json.loads(file.read())
        f.write(str(data["n"]) + "\n")
        steps = data["ops"]
        for step in steps:
            f.write(str(step["p"]) + " " + str(step["x"]) + " " + str(step["y"]) + " " + str(step["s"]) + "\n")
