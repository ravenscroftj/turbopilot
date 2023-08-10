#ifndef __TURBOPILOT_REPLIT_H
#define __TURBOPILOT_REPLIT_H

#include <turbopilot/model.hpp>

#include <vector>
#include <map>
#include <unordered_map>

using piece_t = std::pair<std::size_t, float>;
using piece_map_t = std::unordered_map<std::string, piece_t>;

struct replit_tokenizer {
    gpt_vocab raw_vocab;
    piece_map_t piece_map;
    std::vector<std::string> vocab;
};

// no defaults for now
struct replit_hparams {
    int32_t d_model = 0;
    int32_t max_seq_len = 0;
    int32_t n_heads = 0;
    int32_t n_layers = 0;
    int32_t n_vocab = 0;
    int32_t ftype = 0;
};

struct replit_layer {
    // pre normalization
    struct ggml_tensor * norm_1_weight;

    // attention
    struct ggml_tensor * c_attn_wqkv_weight;
    struct ggml_tensor * c_attn_out_proj_weight;

    // post normalization
    struct ggml_tensor * norm_2_weight;

    // ff
    struct ggml_tensor * ffn_up_proj;
    struct ggml_tensor * ffn_down_proj;
};

struct replit_model {
    replit_hparams hparams;

    struct ggml_tensor * wte_weight;    // position embedding
    struct ggml_tensor * norm_f_weight; // language model head

    std::vector<replit_layer> layers;

    // key + value memory
    struct ggml_tensor * memory_k;
    struct ggml_tensor * memory_v;

    struct ggml_context * ctx;
    std::map<std::string, struct ggml_tensor *> tensors;
};



class ReplitModel : public TurbopilotModel {

public:
    ReplitModel(ModelConfig config, std::mt19937 &rng) : TurbopilotModel(config, rng){
        model = replit_model{};
        vocab = replit_tokenizer{};
    }
    virtual ~ReplitModel();
    bool load_model(std::string path);
    virtual std::stringstream predict(std::string prompt, int max_length, bool include_prompt);

private:
    replit_model model;
    replit_tokenizer vocab;


};

#endif //__TURBOPILOT_REPLIT_H