#ifndef __TURBOPILOT_MODEL_H
#define __TURBOPILOT_MODEL_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <random>
#include <mutex>

typedef void (*offload_func_t)(struct ggml_tensor * tensor);
void ggml_nop(struct ggml_tensor * tensor);

struct gpt_vocab
{
    using id = int32_t;
    using token = std::string;

    std::map<token, id> token_to_id;
    std::map<id, token> id_to_token;
    std::vector<std::string> special_tokens;

    void add_special_token(const std::string &token);
};

std::vector<gpt_vocab::id> gpt_tokenize(const gpt_vocab &vocab, const std::string &text);

gpt_vocab::id gpt_sample_top_k_top_p(
    const gpt_vocab &vocab,
    const float *logits,
    int top_k,
    double top_p,
    double temp,
    std::mt19937 &rng);

struct ModelConfig
{
    int n_threads = 4;
    int32_t top_k = 40;
    float top_p = 0.95f;
    float temp = 0.80f;
    float repeat_penalty = 1.10f;
    int32_t seed = -1;     // RNG seed
    int32_t n_ctx = 512;   // context size
    int32_t n_batch = 512; // batch size for prompt processing (must be >=32 to use BLAS)
    int32_t n_gpu_layers = 0;
};

class TurbopilotModel
{

public:
    TurbopilotModel(ModelConfig config, std::mt19937 &rng) : 
        config(config), 
        rng(rng) 
    {}
    virtual bool load_model(std::string model_path) = 0;
    std::stringstream predict(std::string prompt, int max_length, bool include_prompt);
    void lock();
    void unlock();


protected:
    virtual std::stringstream predict_impl(std::string prompt, int max_length, bool include_prompt) = 0;
    ModelConfig config;
    std::mt19937 &rng;
    std::mutex model_lock;
};

#endif //__TURBOPILOT_MODEL_H