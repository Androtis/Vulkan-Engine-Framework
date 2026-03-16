#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <vector>
#include <iostream>
#include <stdexcept>

#include <cstring>
#include <cstdlib>

//Proxy function to create debug util messengers
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
        "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr){
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
     }
}

//Proxy function to destroy debug util messengers
void DestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
        "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr){
        func(instance, debugMessenger, pAllocator);
    }
}

// Class for handling the computer graphics triangle
class HelloTriangleApplication {
public:
    //Run
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    //VARIABLES
    //GLFW Window
    GLFWwindow* window;
    //Height of the window
    const uint32_t HEIGHT = 600;
    //Width of the window
    const uint32_t WIDTH = 800;


    //Validation Layer Support
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    // DEBUG to either turn on or off, the validation layers
    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif // !NDEBUG

    //Vulkan Instance
    VkInstance instance;

    //Debug Callback
    VkDebugUtilsMessengerEXT debugMessenger;
    

    //Functions

    //Initalizes the GLFW Window
    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulk-an", nullptr, nullptr);
    }

    //Handles debug callbacks
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    //Returns all required extensions
    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions,
            glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
            
        return extensions;
    }

    //Checks if the application has validation layer support
    bool checkValidationLayerSupport() {
        // Set up a vector containing all instance layer properties

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // Loops through all sought after validation layers and checks if
        // they are available

        for (const char* layerName : validationLayers){
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0){
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) { return false; }
        }

        return true;
    }



    //Creates the Vulkan Instance
    void createInstance() {
        //Checks validation layer support
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }


        //Describe application info
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello, Triangle";
        appInfo.applicationVersion =  VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //Create instance info off of the application info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;


        //Create information Extensions count set-up
        std::vector<const char*> extensions = getRequiredExtensions();

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount = 0;

        //Create information validation layer set-up
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        //Create the vulkan instance
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

        //Check all the extensions available
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extenstions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extenstions.data());
        
        std::cout << "available extensions\n";

        for(const auto& extenstion : extenstions) {
            std::cout << "\t" << extenstion.extensionName << "\n";
        }

        //MoltenVK incompatiable error fix
        /*
        std::vector<const char*> requiredExtensions;

        for (uint32_t i = 0; i < glfwExtensionCount; i++)
            requiredExtensions.emplace_back((glfwExtensions[i]));

        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        if (vkCreateInstance(&createInfo, nullptr, &instance))
            throw std::runtime_error("failed to create instance!");
        */
    }

    //Handles all vulkan set-up
    void initVulkan() {
        createInstance();
        setupDebugMessenger();
    }

    //Populates the debug messenger create info with data
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo){
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;
    }

    //Sets up the debug messenger
    void setupDebugMessenger(){
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, 
            nullptr, &debugMessenger) != VK_SUCCESS){
            throw std::runtime_error("failed to setup debug messenger");
        }

    }

    //The rendering loop
    void mainLoop() {
        // Consistenly polls for window input events
        while (!glfwWindowShouldClose(window)){
            glfwPollEvents();
        }
    }

    //Safely cleans up the application
    void cleanup() {
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }
};

// Our running code
int main() {
    //Creates the hello triangle app
    HelloTriangleApplication app;

    //Runs the main loop
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
