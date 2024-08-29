# swagger doc: https://proconvn.duckdns.org/docs

import requests
import json

url = "https://proconvn.duckdns.org"
headers = {"Authorization": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6MTUsIm5hbWUiOiJibHVlY2F0IiwiaXNfYWRtaW4iOmZhbHNlLCJpYXQiOjE3MjQ4MDc1MDYsImV4cCI6MTcyNDk4MDMwNn0.D89kJUMk6RF7-7zbUxszJwJDY78OetESZopSfGmZSyE"}

# request question from server
question_id = 29
question = requests.get(f"{url}/question/{question_id}", headers=headers).json()
a = json.loads(question["question_data"])["board"]["start"] + json.loads(question["question_data"])["board"]["goal"]
b = json.loads(question["question_data"])["board"]
#print(b["height"])
#for value in b: print(value)
#print(b)

with open("input.txt", "w") as file:
    file.writelines(str(b["width"]) + "\n")
    file.writelines(str(b["height"]) + "\n")
    for c in a:
        result_str = " ".join(map(str, c))
        file.writelines(result_str + "\n")
    print("OK")


# solve question
def solve(question):
    return {"n": 1, "ops": [{"x": 0, "y": 1, "s": 1, "p": 1}]}


steps = solve(question)

# send your answer to server
payload = {"question_id": question_id, "answer_data": steps}



# res = requests.post(f"{url}/answer", json=payload, headers=headers).json()
# print(res)
# #  get your answer id
# answer_id = res["id"]

# # get your answer and score from server
# answer = requests.get(f"{url}/answer/{answer_id}", headers=headers).json()
# score_data = json.loads(answer["score_data"])
# print("final score:", score_data["final_score"])
