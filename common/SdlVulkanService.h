/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "UpdateService.h"

namespace astu {

    class SdlVulkanService : public UpdatableBaseService {
    public:

        /**
         * Constructor.
         * 
         * @param priority    the priority used to update this service
         */
        SdlVulkanService(int priority = 0);

    protected:

        // Inherited via UpdatableBaseService
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
        virtual void OnUpdate() override;


    private:
        /** The Vulkan instance. */
        VkInstance vkInstance;


        void Cleanup();
        std::vector<std::string> GetRequiredVulkanExtensions() const;
        std::vector<std::string> GetAvailableVulkanLayers() const;
        void CreateVulkanInstance();
        std::string MakeCsv(const std::vector<std::string> & names) const;
        std::vector<const char*> MakePointerList(const std::vector<std::string> & names) const;
        void LogVulkanInstanceVersion() const;
    };


} // end of namespace