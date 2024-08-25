env: C:\Users\1\qwbt\AIAgent\AIAgentvenc\Script\activate
openai == 0.28.1
langchain == 0.2.14
jupyterlab == 4.2.4
dashscope == 1.20.4
python version == 3.10.11
pipdeptree // check installed pkgs
pip list > installed_packages.txt
open jupyternote book : (AIAgentvenc) C:\Users\1\qwbt\AIAgent > jupyter-lab

langchain codes：
langchain.predict 根据文本提示来生成新的文本
langchain. prompts 将提示词模板化
langchain.schema 允许用户自定义输出解释器

openai key：
import os 
os.environ ["OPENAI_API_KEY"']="sk-9npZpELUzM0IxRU1B79bF5B9FfC04bDeA730F4090dF782A3" 
os.environ ["OPENAI_PROXY"'] = "https://ai-yyds.com/v1"

Ali Qwen api key:
sk-e3c606cb381b4cba9d8dded2d714e071