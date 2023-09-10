#!/bin/sh
if [ -z "$GPU_LAYERS" ]; then 
    /app/turbopilot -t $THREADS -m $MODEL_TYPE -f $MODEL 
else
    /app/turbopilot -t $THREADS -m $MODEL_TYPE -f $MODEL --ngl $GPU_LAYERS
fi