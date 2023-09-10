#ifndef __TURBOPILOT_GPTNEOX_H
#define __TURBOPILOT_GPTNEOX_H

#include <turbopilot/model.hpp>

#include <vector>
#include <map>

// default hparams (StableLM 3B)
struct gpt_neox_hparams {
    int32_t n_vocab = 50257;
    int32_t n_ctx   = 4096;
    int32_t n_embd  = 4096;
    int32_t n_head  = 32;
    int32_t n_layer = 16;
    int32_t n_rot   = 32; // rotary_pct * (n_embd / n_head)
    int32_t par_res = 1; // 1 = true, 0 = false
    int32_t ftype   = 1;
};

struct gpt_neox_layer {
    // pre normalization
    struct ggml_tensor * ln_1_g;
    struct ggml_tensor * ln_1_b;

    // attention
    struct ggml_tensor * c_attn_attn_w;
    struct ggml_tensor * c_attn_attn_b;

    struct ggml_tensor * c_attn_proj_w;
    struct ggml_tensor * c_attn_proj_b;

    // post normalization
    struct ggml_tensor * ln_2_g;
    struct ggml_tensor * ln_2_b;

    // ff
    struct ggml_tensor * c_mlp_fc_w;
    struct ggml_tensor * c_mlp_fc_b;

    struct ggml_tensor * c_mlp_proj_w;
    struct ggml_tensor * c_mlp_proj_b;
};

struct gpt_neox_model {
    gpt_neox_hparams hparams;

    // normalization
    struct ggml_tensor * ln_f_g;
    struct ggml_tensor * ln_f_b;

    struct ggml_tensor * wte; // position embedding

    struct ggml_tensor * lmh_g; // language model head
    //struct ggml_tensor * lmh_b; // language model bias

    std::vector<gpt_neox_layer> layers;

    // key + value memory
    struct ggml_tensor * memory_k;
    struct ggml_tensor * memory_v;

    //
    struct ggml_context * ctx;
    std::map<std::string, struct ggml_tensor *> tensors;
};


class GPTNEOXModel : public TurbopilotModel {

public:
    GPTNEOXModel(ModelConfig config, std::mt19937 &rng) : TurbopilotModel(config, rng){
        this->model = new gpt_neox_model{};
        this->vocab = new gpt_vocab{};
    }
    virtual ~GPTNEOXModel();
    bool load_model(std::string path);
    virtual std::stringstream predict_impl(std::string prompt, int max_length, bool include_prompt);

private:
    gpt_neox_model *model = NULL;
    gpt_vocab *vocab = NULL;


};

#endif // __TURBOPILOT_GPTNEOX_H