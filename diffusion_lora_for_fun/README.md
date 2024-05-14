[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/X3WkcXtG)


此次作業在執行時我是用

python 3.10.9	torch 2.2.1	cuda12.1執行


1. 選擇一個hugging face上的stable diffusion，我是用runwayml/stable-diffusion-v1-5這個pre-train的模型

2. 然後用cmd下這三行指令，把lora抓下來，並且進到diffuser下載他需要的東西 

   git clone https://github.com/huggingface/diffusers
   cd diffusers
   pip install .

3. 再進兩層，下這兩行指令，下載等等要用的train_text_to_image_lora.py需要的東西

   cd examples/text_to_image
   pip install -r requirements.txt

4. 然後，上網抓16張訓練資料(文件夾名稱 training_dataset)，下載完之後，我有寫一個jsonl檔，把你的圖檔對到一串prompt，好像是因為他是text_to_image，所以會用那張圖對應到的文字還有原圖去做訓練(不確定)，然後下以下指令，即可開始訓練。

   注意: 這時文件夾要在 "diffusers\examples\text_to_image" 那一層

   accelerate launch train_text_to_image_lora.py \ 
   --pretrained_model_name_or_path="runwayml/stable-diffusion-v1-5" \
   --dataset_name="path_to_training_dataset" \
   --dataloader_num_workers=0 --resolution=512 --center_crop --random_flip \
   --train_batch_size=1 --gradient_accumulation_steps=1 --max_train_steps=200 \
   --learning_rate=1e-04 --max_grad_norm=1 --lr_scheduler="linear" --lr_warmup_steps=0 \
   --output_dir="path_to_store_output_result" --report_to=wandb \
   --checkpointing_steps=20 --validation_prompt="A lion is walking" --seed=1337 

5. 結束訓練後，會在你自己設定的output_dir資料夾裡面看到lora權重，也就是我放的 "pytorch_lora_weights.safetensors"，把權重路徑給到diffusion_lora.py裡面，自己下prompt，即可生成出圖片。

結束...