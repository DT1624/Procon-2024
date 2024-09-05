# swagger doc: https://proconvn.duckdns.org/docs

import requests
import json

url = "https://proconvn.duckdns.org"
headers = {"Authorization": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6MTUsIm5hbWUiOiJibHVlY2F0IiwiaXNfYWRtaW4iOmZhbHNlLCJpYXQiOjE3MjU0Njk0ODcsImV4cCI6MTcyNTY0MjI4N30.joypYjjVvvNNIRpc4Pco_93S4zH3-AfqPpcVf_KAWeM"}

question_id = 54
question = requests.get(f"{url}/question/{question_id}", headers=headers).json()
# if "message" not in question: 
a = json.loads(question["question_data"])["board"]["start"] + json.loads(question["question_data"])["board"]["goal"]
b = json.loads(question["question_data"])["board"]
d = json.loads(question["question_data"])["general"]

with open("input.txt", "w") as file:
    file.writelines(str(b["width"]) + "\n")
    file.writelines(str(b["height"]) + "\n")
    for c in a:
        result_str = " ".join(map(str, c))
        file.writelines(result_str + "\n")
    file.write(str(d["n"]) + "\n")
    for c in d["patterns"]:
        file.write(str(c["p"]) + " " + str(c["width"]) + " " + str(c["height"]) + "\n")
        for e in c["cells"]:
            result_str = " ".join(map(str, e))
            file.write(result_str + "\n")
    print("OK")





