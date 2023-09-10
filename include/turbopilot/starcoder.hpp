#ifndef __TURBOPILOT_STARCODER_H
#define __TURBOPILOT_STARCODER_H

#include <turbopilot/model.hpp>

// default hparams (GPT-2 117M)
// https://huggingface.co/bigcode/gpt_bigcode-santacoder/blob/main/config.json
struct starcoder_hparams {
    int32_t n_vocab = 49280;
    int32_t n_ctx   = 2048;
    int32_t n_embd  = 2048;
    int32_t n_head  = 16;
    int32_t n_layer = 24;
    int32_t ftype   = 1;
};

struct starcoder_layer {
    // normalization
    struct ggml_tensor * ln_1_g;
    struct ggml_tensor * ln_1_b;

    struct ggml_tensor * ln_2_g;
    struct ggml_tensor * ln_2_b;

    // attention
    struct ggml_tensor * c_attn_attn_w;
    struct ggml_tensor * c_attn_attn_b;

    struct ggml_tensor * c_attn_proj_w;
    struct ggml_tensor * c_attn_proj_b;

    // mlp
    struct ggml_tensor * c_mlp_fc_w;
    struct ggml_tensor * c_mlp_fc_b;

    struct ggml_tensor * c_mlp_proj_w;
    struct ggml_tensor * c_mlp_proj_b;
};

struct starcoder_model {
    starcoder_hparams hparams;

    // normalization
    struct ggml_tensor * ln_f_g;
    struct ggml_tensor * ln_f_b;

    struct ggml_tensor * wte;     // position embedding
    struct ggml_tensor * wpe;     //    token embedding
    struct ggml_tensor * lm_head; // language model head

    std::vector<starcoder_layer> layers;

    // key + value memory
    struct ggml_tensor * memory_k;
    struct ggml_tensor * memory_v;

    //
    struct ggml_context * ctx;
    std::map<std::string, struct ggml_tensor *> tensors;
};


class StarcoderModel : public TurbopilotModel {
public:
    StarcoderModel(ModelConfig config, std::mt19937 &rng) : TurbopilotModel(config, rng){
        this->model = new starcoder_model{};
        this->vocab = new gpt_vocab{};
    }
    virtual ~StarcoderModel();
    bool load_model(std::string path);
    virtual std::stringstream predict_impl(std::string prompt, int max_length, bool include_prompt);

private:
    starcoder_model *model = NULL;
    gpt_vocab *vocab = NULL;
};


#endif //__TURBOPILOT_STARCODER_H