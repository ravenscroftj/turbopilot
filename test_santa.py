#%%
import torch
from transformers import CodeGenTokenizer, GPTJForCausalLM


checkpoint = "/home/james/workspace/rafael-llm/codegen-2B-multi-gptj"
device = "cuda" # for GPU usage or "cpu" for CPU usage

tokenizer = CodeGenTokenizer.from_pretrained("Salesforce/codegen-350M-multi")
model = GPTJForCausalLM.from_pretrained(checkpoint).to(device)


#model = AutoModel.from_pretrained(checkpoint, trust_remote_code=True).to(device)
#%%

# define the user model
class User:


# %%
code = """import os
import requests

#send the json data to pastebin
def send_data"""
inputs = tokenizer.encode(code, return_tensors="pt").to(device)
outputs = model.generate(inputs, max_length=200)
response = tokenizer.decode(outputs[0])

print(response)

import requests

#send the json data to pastebin
def send_data(data):
    url = "http://pastebin.com/api_post.php"
    data = {"api_dev_key": "<api_key>", "api_user_key": "<user_key>", "api_content": data}
    response = requests.post(url, data=data).text
    return response



# %%
code
# %%
