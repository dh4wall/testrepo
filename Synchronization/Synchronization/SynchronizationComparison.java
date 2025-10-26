package Synchronization;
import java.util.LinkedList;
import java.util.concurrent.Semaphore;

class ProducerConsumerWithMutex {
    LinkedList<Integer> buffer = new LinkedList<>();
    int capacity = 2;
    final Object mutex = new Object();
    int maxIterations = 5;

    public void produce() throws InterruptedException {
        int value = 0;
        for (int i = 0; i < maxIterations; i++) {
            synchronized (mutex) {
                if (buffer.size() == capacity) {
                    System.out.println("Producer waiting — Buffer Full");
                }
                while (buffer.size() == capacity) {
                    mutex.wait();
                }
                buffer.add(value);
                System.out.println("Producer produced: " + value + " | Buffer: " + buffer);
                value++;
                mutex.notify(); // notify consumer
            }
            Thread.sleep(500);
        }
    }

    public void consume() throws InterruptedException {
        for (int i = 0; i < maxIterations; i++) {
            synchronized (mutex) {
                if (buffer.isEmpty()) {
                    System.out.println("Consumer waiting — Buffer Empty");
                }
                while (buffer.isEmpty()) {
                    mutex.wait();
                }
                int val = buffer.removeFirst();
                System.out.println("Consumer consumed: " + val + " | Buffer: " + buffer);
                mutex.notify(); // notify producer
            }
            Thread.sleep(1000);
        }
    }
}

class ProducerConsumerWithSemaphore {
    LinkedList<Integer> buffer = new LinkedList<>();
    int capacity = 2;
    Semaphore empty = new Semaphore(capacity);
    Semaphore full = new Semaphore(0);
    Semaphore mutex = new Semaphore(1);
    int maxIterations = 5;

    public void produce() throws InterruptedException {
        int value = 0;
        for (int i = 0; i < maxIterations; i++) {
            empty.acquire();
            mutex.acquire();
            if (buffer.size() == capacity) {
                System.out.println("Producer waiting — Buffer Full");
            }
            buffer.add(value);
            System.out.println("Producer produced: " + value + " | Buffer: " + buffer);
            value++;
            mutex.release();
            full.release();
            Thread.sleep(500);
        }
    }

    public void consume() throws InterruptedException {
        for (int i = 0; i < maxIterations; i++) {
            full.acquire();
            mutex.acquire();
            if (buffer.isEmpty()) {
                System.out.println("Consumer waiting — Buffer Empty");
            }
            int val = buffer.removeFirst();
            System.out.println("Consumer consumed: " + val + " | Buffer: " + buffer);
            mutex.release();
            empty.release();
            Thread.sleep(1000);
        }
    }
}

public class SynchronizationComparison {

    public static void main(String[] args) throws InterruptedException {

        System.out.println("=== Running Producer-Consumer with Mutex ===");
        ProducerConsumerWithMutex pcMutex = new ProducerConsumerWithMutex();

        Thread consumerMutex = new Thread(() -> {
            try {
                pcMutex.consume();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });

        Thread producerMutex = new Thread(() -> {
            try {
                pcMutex.produce();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });

        // Start consumer first
        consumerMutex.start();
        Thread.sleep(200);
        producerMutex.start();

        producerMutex.join();
        consumerMutex.join();

        System.out.println("\n=== Running Producer-Consumer with Semaphore ===");
        ProducerConsumerWithSemaphore pcSemaphore = new ProducerConsumerWithSemaphore();

        Thread consumerSemaphore = new Thread(() -> {
            try {
                pcSemaphore.consume();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });

        Thread producerSemaphore = new Thread(() -> {
            try {
                pcSemaphore.produce();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });

        // Start consumer first
        consumerSemaphore.start();
        Thread.sleep(200);
        producerSemaphore.start();

        producerSemaphore.join();
        consumerSemaphore.join();
    }
}


  