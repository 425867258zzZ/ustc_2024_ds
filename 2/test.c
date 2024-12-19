#include <stdio.h>

int maxSubArray(int *nums, int numsSize)
{
    int curSum, maxSum;
    curSum = maxSum = nums[0];
    for (int i = 1; i < numsSize; i++)
    {
        curSum = curSum + nums[i] > nums[i] ? curSum + nums[i] : nums[i];
        maxSum = maxSum > curSum ? maxSum : curSum;
    }
    return maxSum;
}

int main()
{
    int nums[9] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    printf("%d", maxSubArray(nums, 9));
    return 0;
}