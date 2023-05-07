// SPDX-FileCopyrightText: Copyright 2023 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <forward_list>
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <utility>
#include <QString>
#include <QWidget>
#include "common/settings.h"
#include "yuzu/configuration/shared_translation.h"

namespace ConfigurationShared {

std::unique_ptr<TranslationMap> InitializeTranslations(QWidget* parent) {
    std::unique_ptr<TranslationMap> translations = std::make_unique<TranslationMap>();
    const auto& tr = [parent](const char* text) -> QString { return parent->tr(text); };

#define INSERT(LABEL, NAME, TOOLTIP)                                                               \
    translations->insert(std::pair{(LABEL), std::pair{tr((NAME)), tr((TOOLTIP))}})

    // Audio
    INSERT("output_engine", "Output Engine:", "");
    INSERT("output_device", "Output Device:", "");
    INSERT("input_device", "Input Device:", "");
    INSERT("audio_muted", "Mute audio when in background", "");
    INSERT("volume", "Volume:", "");

    // Core
    INSERT("use_multi_core", "Multicore CPU Emulation", "");
    INSERT("use_unsafe_extended_memory_layout", "Unsafe extended memory layout (8GB DRAM)", "");

    // Cpu
    INSERT("cpu_accuracy", "Accuracy:", "");
    INSERT("cpu_debug_mode", "Enable CPU Debugging", "");
    INSERT("cpuopt_page_tables", "Enable inline page tables", "");
    INSERT("cpuopt_block_linking", "Enable block linking", "");
    INSERT("cpuopt_return_stack_buffer", "Enable return stack buffer", "");
    INSERT("cpuopt_fast_dispatcher", "Enable fast dispatcher", "");
    INSERT("cpuopt_context_elimination", "Enable context elimination", "");
    INSERT("cpuopt_const_prop", "Enable constant propagation", "");
    INSERT("cpuopt_misc_ir", "Enable miscellaneous optimizations", "");
    INSERT("cpuopt_reduce_misalign_checks", "Enable misalignment check reduction", "");
    INSERT("cpuopt_fastmem", "Enable Host MMU Emulation (general memory instructions)", "");
    INSERT("cpuopt_fastmem_exclusives", "Enable Host MMU Emulation (exclusive memory instructions)",
           "");
    INSERT("cpuopt_recompile_exclusives", "Enable recompilation of exlucsive memory instructions",
           "");
    INSERT("cpuopt_ignore_memory_aborts", "Enable fallbacks for invalid memory accesses", "");

    INSERT("cpuopt_unsafe_unfuse_fma", "Unfuse FMA (improve performance on CPUs without FMA)", "");
    INSERT("cpuopt_unsafe_reduce_fp_error", "Faster FRSQRTE and FRECPE", "");
    INSERT("cpuopt_unsafe_ignore_standard_fpcr", "Faster ASIMD instructions (32 bits only)", "");
    INSERT("cpuopt_unsafe_inaccurate_nan", "Inaccurate NaN handling", "");
    INSERT("cpuopt_unsafe_fastmem_check", "Disable address space checks", "");
    INSERT("cpuopt_unsafe_ignore_global_monitor", "Ignore global monitor", "");

    // Renderer
    INSERT("backend", "API:", "");
    INSERT("vulkan_device", "Device:", "");
    INSERT("shader_backend", "Shader Backend:", "");
    INSERT("resolution_setup", "Resolution:", "");
    INSERT("scaling_filter", "Window Adapting Filter:", "");
    INSERT("fsr_sharpening_slider", "AMD FidelityFX™ Super Resolution Sharpness:", "");
    INSERT("anti_aliasing", "Anti-Aliasing Method:", "");
    INSERT("fullscreen_mode", "Fullscreen Mode:", "");
    INSERT("aspect_ratio", "Aspect Ratio:", "");
    INSERT("use_disk_shader_cache", "Use disk pipeline cache", "");
    INSERT("use_asynchronous_gpu_emulation", "Use asynchronous GPU emulation", "");
    INSERT("nvdec_emulation", "NVDEC emulation:", "");
    INSERT("acclerate_astc", "ASTC Decoding Method:", "");
    INSERT(
        "use_vsync", "VSync Mode:",
        "FIFO (VSync) does not drop frames or exhibit tearing but is limited by the screen refresh "
        "rate. FIFO Relaxed is similar to FIFO but allows tearing as it recovers from a slow down. "
        "Mailbox can have lower latency than FIFO and does not tear but may drop frames. Immediate "
        "(no synchronization) just presents whatever is available and can exhibit tearing.");

    // Renderer (Advanced Graphics)
    INSERT("async_presentation", "Enable asynchronous presentation (Vulkan only)", "");
    INSERT("force_max_clock", "Force maximum clocks (Vulkan only)",
           "Runs work in the background while waiting for graphics commands to keep the GPU from "
           "lowering its clock speed.");
    INSERT("max_anisotropy", "Anisotropic Filtering:", "");
    INSERT("gpu_accuracy", "Accuracy Level:", "");
    INSERT("use_asynchronous_shaders", "Use asynchronous shader building (Hack)",
           "Enables asynchronous shader compilation, which may reduce shader stutter. This feature "
           "is experimental.");
    INSERT("use_fast_gpu_time", "Use Fast GPU Time (Hack)",
           "Enables Fast GPU Time. This option will force most games to run at their highest "
           "native resolution.");
    INSERT("use_vulkan_driver_pipeline_cache", "Use Vulkan pipeline cache",
           "Enables GPU vendor-specific pipeline cache. This option can improve shader loading "
           "time significantly in cases where the Vulkan driver does not store pipeline cache "
           "files internally.");
    INSERT("enable_compute_pipelines", "Enable Compute Pipelines (Intel Vulkan Only)",
           "Enable compute pipelines, required by some games.\nThis setting only exists for Intel "
           "proprietary drivers, and may crash if enabled.\nCompute pipelines are always enabled "
           "on all other drivers.");
    INSERT("bg_red", "Background Color:", "");
    INSERT("bg_green", "Background Color:", "");
    INSERT("bg_blue", "Background Color:", "");

    // Renderer (Debug)
    INSERT("debug", "Enable Graphics Debugging",
           "When checked, the graphics API enters a slower debugging mode");
    INSERT("shader_feedback", "Enable Shader Feedback",
           "When checked, yuzu will log statistics about the compiled pipeline cache");
    INSERT("nsight_aftermath", "Enable Nsight Aftermath",
           "When checked, it enables Nsight Aftermath crash dumps");
    INSERT("disable_shader_loop_safety_checks", "Disable Loop safety checks",
           "When checked, it executes shaders without loop logic changes");

    // Renderer (General)
    INSERT("use_speed_limit", "Limit Speed Percent", "");
    INSERT("speed_limit", "Limit Speed Percent", "");

    // System
    INSERT("rng_seed_enabled", "RNG Seed", "");
    INSERT("rng_seed", "RNG Seed", "");
    INSERT("device_name", "Device Name", "");
    INSERT("custom_rtc_enabled", "Custom RTC", "");
    INSERT("custom_rtc", "Custom RTC", "");
    INSERT("language_index", "Language:", "");
    INSERT("region_index", "Region:", "");
    INSERT("time_zone_index", "Time Zone:", "");
    INSERT("sound_index", "Sound Output Mode:", "");
    INSERT("use_docked_mode", "Docked", "");

#undef INSERT

    return translations;
}

std::forward_list<QString> ComboboxEnumeration(std::type_index type, QWidget* parent) {
    const auto& tr = [&](const char* text) { return parent->tr(text); };

    if (type == typeid(Settings::AstcDecodeMode)) {
        return {
            tr("CPU"),
            tr("GPU"),
            tr("CPU Asynchronous"),
        };
    } else if (type == typeid(Settings::RendererBackend)) {
        return {
            tr("OpenGL"),
            tr("Vulkan"),
            tr("Null"),
        };
    } else if (type == typeid(Settings::ShaderBackend)) {
        return {
            tr("GLSL"),
            tr("GLASM (Assembly Shaders, NVIDIA Only)"),
            tr("SPIR-V (Experimental, Mesa Only)"),
        };
    } else if (type == typeid(Settings::GPUAccuracy)) {
        return {
            tr("Normal"),
            tr("High"),
            tr("Extreme"),
        };
    } else if (type == typeid(Settings::AnisotropyMode)) {
        return {
            tr("Automatic"), tr("Default"), tr("2x"), tr("4x"), tr("8x"), tr("16x"),
        };
    }

    return {};
}

} // namespace ConfigurationShared
